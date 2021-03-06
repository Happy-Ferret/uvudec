/*
UVNet Universal Decompiler (uvudec)
Copyright 2010 John McMaster <JohnDMcMaster@gmail.com>
Licensed under the terms of the LGPL V3 or later, see COPYING for details
*/

#include "uvdflirt/flirt.h"
#include "uvdflirt/function.h"
#include "uvdflirt/sig/sig.h"
#include "uvdflirt/sig/tree/tree.h"
#include "uvdflirt/sig/writer.h"
#include <string.h>

UVDFLIRTSignatureDBWriter::UVDFLIRTSignatureDBWriter(UVDFLIRTSignatureDB *db)
{
	m_db = db;
	memset(&m_header, 0, sizeof(struct UVD_IDA_sig_header_t));
}

UVDFLIRTSignatureDBWriter::~UVDFLIRTSignatureDBWriter()
{
}

uv_err_t UVDFLIRTSignatureDBWriter::bitshiftAppend(uint32_t data)
{
	printf_flirt_debug("0x%.8X\n", data);
	/*
	int bitshift_read()
		uint32_t first = read_byte();
	
		if ( first & 0x80)
			return ((first & 0x7F) << 8) + read_byte();
		return first;
	*/
	uint8_t byte = 0;

	//15 bit limit, MSB is ignored for escape
	uv_assert_ret(data <= 0x7FFF);

	if( data >= 0x80 )
	{
		//First byte is the high byte
		byte = 0x80 | ((data & 0xFF00) >> 8);
		uv_assert_err_ret(uint8Append(byte));
	}
	else
	{
		byte = data & 0xFF;
		uv_assert_err_ret(uint8Append(byte));
	}
	
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::uint8Append(uint8_t in)
{
	printf_flirt_debug("0x%02X\n", in);
	uv_assert_err_ret(m_tree.appendByte(in));
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::uint16Append(uint16_t in)
{
	uv_assert_err_ret(uint8Append((in & 0xFF00) >> 8));
	uv_assert_err_ret(uint8Append(in & 0xFF));
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::updateHeader()
{
	/*
	char magic[6];
	uint8_t version;
	uint8_t processor;
	uint32_t file_types;
	uint16_t OS_types;
	uint16_t app_types;
	uint8_t feature_flags;
	char pad;
	uint16_t old_number_modules;
	uint16_t crc16;
	char ctype[0x22 - 0x16];
	uint8_t library_name_sz;
	uint16_t alt_ctype_crc;
	uint32_t n_modules;
	*/
	
	//Copy over pre-defined values
	m_header = m_db->m_header;
	memcpy(&m_header.magic, UVD_FLIRT_SIG_MAGIC, UVD_FLIRT_SIG_MAGIC_SIZE);
	
	uint32_t numberFunctions = 0;
	uv_assert_err_ret(m_db->size(&numberFunctions));
	printf_flirt_debug("numberFunctions: 0x%08X\n", numberFunctions);
	uv_assert_ret(numberFunctions != 0);
	m_header.old_number_modules = numberFunctions;
	m_header.n_modules = numberFunctions;

	//FIXME: this needs to be set?
	//Eh I actually don't know what this is calculated from or why its there
	//A file integrity checksum on the tree maybe?
	//I think maybe this was set in previous versions...but I see it seemingly unset in some files
	//So maybe its only set on certain object formats?
	m_header.crc16 = 0;
	
	//FIXME: what does this mean?
	//Looks like its suppose to be null terminated
	//I've seen blank in files, so thats good enough for me
	strcpy(&m_header.ctype[0], "");
	
	//Not null terminated
	m_header.library_name_sz = m_db->m_libraryName.size();

	//Unknown meaning, set to 0
	m_header.alt_ctype_crc = 0;
	
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::updateForWrite()
{
	//Header is allocated in the writter
	
	//Create the sig table
	//Internal node table?
	
	printf_flirt_debug("updateForWrite()\n");

	uv_assert_err_ret(updateHeader());
	
	return UV_ERR_OK;
}

//This is probably just the 32 bit write function, but the only 32 bit var was the mask
uv_err_t UVDFLIRTSignatureDBWriter::constructRelocationBitmask(uint32_t nNodeBytes, uint32_t relocationBitmask)
{
	printf_flirt_debug("nNodeBytes:0x%08X, relocationBitmask:0x%08X\n", nNodeBytes, relocationBitmask);

	if( nNodeBytes >= 0x10 )
	{
		/*
		//relocation_bitmask = read_relocation_bitmask();
		uint32_t first;
		uint32_t lower;
		uint32_t upper;

		first = read_byte();

		if ((first & 0x80) != 0x80)
			return first;

		if ((first & 0xC0) != 0xC0)
			return ((first & 0x7F) << 8) + read_byte();

		if ((first & 0xE0) != 0xE0) {
			upper = ((first & 0xFF3F) << 8) + read_byte();
			lower = read16();
		} else {
			upper = read16();
			lower = read16();
		}
		uint32_t ret = lower + (upper << 16);
		return ret;
		*/
		
		//Strictly speaking, we could be lazy and just use all 32 bit relocation masks
		if( relocationBitmask < 0x80 )
		{
			uv_assert_err_ret(uint8Append(relocationBitmask));
		}
		else if( relocationBitmask < 0x8000 )
		{
			uv_assert_err_ret(uint8Append(((relocationBitmask >> 8) & 0x7F) | 0x80));
			uv_assert_err_ret(uint8Append(relocationBitmask & 0xFF));
		}
		else if( relocationBitmask < 0x40000000 )
		{
			/*
			upper = ((first & 0x3F) << 8) + read_byte();
			lower = read16();
			*/
			
			//First byte is high order
			uv_assert_err_ret(uint8Append(((relocationBitmask >> 24) & 0x7F) | 0xC0));
			uv_assert_err_ret(uint8Append((relocationBitmask >> 16) & 0xFF));
			uv_assert_err_ret(uint16Append(relocationBitmask & 0xFFFF));
		}
		else
		{
			//Escape prefix
			uv_assert_err_ret(uint8Append(0xE0));
			//Followed by the bytes
			uv_assert_err_ret(uint16Append(relocationBitmask >> 16));
			uv_assert_err_ret(uint16Append(relocationBitmask & 0xFFFF));
		}
	}
	else
	{
		//relocation_bitmask = bitshift_read();
		uv_assert_err_ret(bitshiftAppend(relocationBitmask));
	}
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::getRelocationBitmask(UVDFLIRTSignatureTreeLeadingNode *node, uint32_t *out)
{
	/*
	First byte shoudl be MSB
	*/
	
	uint32_t cur = 0;
	uint32_t ret = 0;
	
	cur = 1 << (node->m_bytes.size() - 1);
	for( UVDFLIRTSignatureRawSequence::iterator iter = node->m_bytes.begin(); iter != node->m_bytes.end(); )
	{
		if( (*iter).m_isReloc )
		{
			ret |= cur;
		}
		cur >>= 1;
		uv_assert_err_ret(iter.next());
	}
	
	uv_assert_ret(out);
	*out = ret;

	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::constructLeadingNodeItem(UVDFLIRTSignatureTreeLeadingNode *node)
{
	uint32_t nNodeBytes = 0;
	uint32_t relocationBitmask = 0;

	uv_assert_ret(node);
	nNodeBytes = node->m_bytes.size();
	
	uv_assert_ret(nNodeBytes <= UVD_FLIRT_SIG_LEADING_LENGTH);
	//n_node_bytes = read_byte();
	printf_flirt_debug("nNodeBytes: 0x%02X, node: %s\n", nNodeBytes, node->m_bytes.toString().c_str());
	uv_assert_err_ret(uint8Append(nNodeBytes));
	
	uv_assert_err_ret(getRelocationBitmask(node, &relocationBitmask));
	printf_flirt_debug("relocation bitmask: 0x%08X\n", relocationBitmask);
	uv_assert_err_ret(constructRelocationBitmask(nNodeBytes, relocationBitmask));
	
	//Dump all of the non-relocation bytes
	printf_flirt_debug("non-relocation bytes\n");
	for( UVDFLIRTSignatureRawSequence::iterator iter = node->m_bytes.begin(); iter != node->m_bytes.end(); )
	{
		if( !(*iter).m_isReloc )
		{
			uv_assert_err_ret(uint8Append((*iter).m_byte));
		}
		uv_assert_err_ret(iter.next());
	}
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::constructLeadingNodeRecurse(UVDFLIRTSignatureTreeLeadingNode *node)
{
	uint32_t nLeadingChildren = 0;

	nLeadingChildren = node->m_leadingChildren.size();
	printf_flirt_debug("nLeadingChildren: %d\n", nLeadingChildren);
	if( nLeadingChildren )
	{
		uv_assert_err_ret(bitshiftAppend(nLeadingChildren));

		//for (int i = 0; i < n_internal_nodes; ++i) {
		for( UVDFLIRTSignatureTreeLeadingNode::LeadingChildrenSet::iterator iter = node->m_leadingChildren.begin(); iter != node->m_leadingChildren.end(); ++iter )
		{
			UVDFLIRTSignatureTreeLeadingNode *cur = *iter;
			uv_assert_err_ret(constructLeadingNodeItem(cur));
			uv_assert_err_ret(constructLeadingNode(cur));
		}
	}

	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::constructCRCNode(UVDFLIRTSignatureTreeLeadingNode *node)
{
	printf_flirt_debug("CRC nodes\n");
	if( node->m_crcNodes.m_nodes.empty() )
	{
		return UV_ERR_OK;
	}
	printf_flirt_debug("CRC nodes start (0 children marker)\n");
	uv_assert_err_ret(bitshiftAppend(0));

	for( UVDFLIRTSignatureTreeHashNodes::HashSet::iterator hashIter = node->m_crcNodes.m_nodes.begin(); hashIter != node->m_crcNodes.m_nodes.end(); ++hashIter )
	{
		UVDFLIRTSignatureTreeHashNode *hashNode = *hashIter;
		/*
		uint32_t tree_block_len = read_byte();
		uint32_t a_crc16 = read16();
		*/
		uv_assert_ret(hashNode);
		printf_flirt_debug("leading length: 0x%02X\n", hashNode->m_leadingLength);
		uv_assert_err_ret(uint8Append(hashNode->m_leadingLength));
		printf_flirt_debug("CRC16: 0x%02X\n", hashNode->m_crc16);
		uv_assert_err_ret(uint16Append(hashNode->m_crc16));
		for( UVDFLIRTSignatureTreeHashNode::BasicSet::iterator basicIter = hashNode->m_bucket.begin(); basicIter != hashNode->m_bucket.end(); ++basicIter )
		{
			UVDFLIRTSignatureTreeBasicNode *basicNode = *basicIter;
			std::vector<UVDFLIRTSignatureReference> allReferences;
			
			uv_assert_ret(basicNode);
			//total_len = bitshift_read();
			uv_assert_err_ret(bitshiftAppend(basicNode->m_totalLength));

			allReferences = basicNode->m_references;
			//Prepend the public names
			for( std::vector<UVDFLIRTPublicName>::iterator nameIter = basicNode->m_publicNames.begin(); nameIter != basicNode->m_publicNames.end(); ++nameIter )
			{
				UVDFLIRTPublicName publicName = *nameIter;
				UVDFLIRTSignatureReference ref;

				ref.m_offset = publicName.getOffset();
				ref.m_name = publicName.m_name;
				//FIXME: figure out what these do
				ref.m_attributeFlags = 0;

				allReferences.insert(allReferences.begin(), ref);								
			}

			//Multiple public names need to be supported
			//Public names are often offset 0, but often not in the case of multiple
			for( std::vector<UVDFLIRTSignatureReference>::iterator refIter = allReferences.begin(); refIter != allReferences.end(); ++refIter )
			{
				UVDFLIRTSignatureReference ref = *refIter;
				uint32_t flags = 0;

				//Public names are at offset 0
				printf_flirt_debug("ref offset: 0x%04X\n", ref.m_offset);
				uv_assert_err_ret(bitshiftAppend(ref.m_offset));
				
				//Write string
				printf_flirt_debug("ref name: %s\n", ref.m_name.c_str());
				for( std::string::iterator iterName = ref.m_name.begin(); iterName != ref.m_name.end(); ++iterName )
				{
					char c = *iterName;
					
					//Chars less than 0x20 (space) need to be escaped
					if( c <= UVD_FLIRT_SIG_NAME_ESCAPE_MAX )
					{
						//Somewhat arbitrary which char we use I guess
						uv_assert_err_ret(uint8Append(c));
					}
					uv_assert_err_ret(uint8Append(c));
				}
				
				//String terminator should include next flags
				//Note these flags are all less than 0x20
				/*
				#define UVD_FLIRT_SIG_NAME_MORE_NAMES					0x01
				#define UVD_FLIRT_SIG_NAME_MORE_BASIC					0x08
				#define UVD_FLIRT_SIG_NAME_MORE_HASH					0x10
				*/
				if( (refIter + 1) != allReferences.end() )
				{
					flags |= UVD_FLIRT_SIG_NAME_MORE_NAMES;
				}
				UVDFLIRTSignatureTreeHashNode::BasicSet::iterator basicIterNext = basicIter;
				++basicIterNext;
				if( basicIterNext != hashNode->m_bucket.end() )
				{
					flags |= UVD_FLIRT_SIG_NAME_MORE_BASIC;
				}
				UVDFLIRTSignatureTreeHashNodes::HashSet::iterator hashIterNext = hashIter;
				++hashIterNext;
				if( hashIterNext != node->m_crcNodes.m_nodes.end() )
				{
					flags |= UVD_FLIRT_SIG_NAME_MORE_HASH;
				}
				printf_flirt_debug("ref write flag: 0x%02X\n", flags);
				uv_assert_err_ret(uint8Append(flags));				
			}
		}
	}
	printf_flirt_debug("crc nodes done\n");

	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::constructLeadingNode(UVDFLIRTSignatureTreeLeadingNode *node)
{
	//Leading prefixes should be resolved first, but we also print shorter nodes first
	
	//If we have internal leading nodes, we write the number of nodes
	//If we have leaf leading nodes, we write 0

	//Shortest should go first, so dump children first if they exist
	printf_flirt_debug("node sub-content\n");
	uv_assert_err_ret(constructCRCNode(node));
	uv_assert_err_ret(constructLeadingNodeRecurse(node));

	UVDFLIRTSignatureRawSequence m_bytes;
		
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::constructTree()
{
	//This seems to have been designed for easy iterative one pass write
	
	//Its gotta start somewhere
	printf_flirt_debug("Constructing tree w/ root node children: %d\n", m_db->m_tree->m_leadingChildren.size());
	uv_assert_err_ret(constructLeadingNode(m_db->m_tree));
	printf_flirt_debug("tree done\n");
	
	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::construct()
{	
	printf_flirt_debug("construct()\n");

	uv_assert_err_ret(addRelocatableData((char *)&m_header, sizeof(m_header)));
	uv_assert_err_ret(addRelocatableData((char *)m_db->m_libraryName.c_str(), m_db->m_libraryName.size()));
	uv_assert_err_ret(constructTree());
	uv_assert_err_ret(addRelocatableData(&m_tree));

	return UV_ERR_OK;
}

uv_err_t UVDFLIRTSignatureDBWriter::applyRelocations()
{
	printf_flirt_debug("applyRelocations()\n");

	//uv_assert_err_ret(applyHeaderRelocations());
	return UV_ERR_OK;
}


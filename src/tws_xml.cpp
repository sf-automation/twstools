#include "tws_xml.h"

#include "debug.h"
#include "twsUtil.h"
#include "ibtws/Contract.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>





#define ADD_ATTR_INT( _struct_, _attr_ ) \
	if( !skip_defaults || _struct_._attr_ != dflt._attr_ ) { \
		snprintf(tmp, sizeof(tmp), "%d",_struct_._attr_ ); \
		xmlNewProp ( ne, (xmlChar*) #_attr_, (xmlChar*) tmp ); \
	}

#define ADD_ATTR_LONG( _struct_, _attr_ ) \
	if( !skip_defaults || _struct_._attr_ != dflt._attr_ ) { \
		snprintf(tmp, sizeof(tmp), "%ld",_struct_._attr_ ); \
		xmlNewProp ( ne, (xmlChar*) #_attr_, (xmlChar*) tmp ); \
	}

#define ADD_ATTR_DOUBLE( _struct_, _attr_ ) \
	if( !skip_defaults || _struct_._attr_ != dflt._attr_ ) { \
		snprintf(tmp, sizeof(tmp), "%.10g", _struct_._attr_ ); \
		xmlNewProp ( ne, (xmlChar*) #_attr_, (xmlChar*) tmp ); \
	}

#define ADD_ATTR_BOOL( _struct_, _attr_ ) \
	if( !skip_defaults || _struct_._attr_ != dflt._attr_ ) { \
		xmlNewProp ( ne, (xmlChar*) #_attr_, \
			(xmlChar*) (_struct_._attr_ ? "1" : "0") ); \
	}

#define ADD_ATTR_STRING( _struct_, _attr_ ) \
	if( !skip_defaults || _struct_._attr_ != dflt._attr_ ) { \
		xmlNewProp ( ne, (xmlChar*) #_attr_, \
			(xmlChar*) _struct_._attr_.c_str() ); \
	}

void conv_ib2xml( xmlNodePtr parent, const IB::Contract& c, bool skip_defaults )
{
	char tmp[128];
	static const IB::Contract dflt;
	
	xmlNodePtr ne = xmlNewChild( parent, NULL, (xmlChar*)"IBContract", NULL);
	
	ADD_ATTR_LONG( c, conId );
	ADD_ATTR_STRING( c, symbol );
	ADD_ATTR_STRING( c, secType );
	ADD_ATTR_STRING( c, expiry );
	ADD_ATTR_DOUBLE( c, strike );
	ADD_ATTR_STRING( c, right );
	ADD_ATTR_STRING( c, multiplier );
	ADD_ATTR_STRING( c, exchange );
	ADD_ATTR_STRING( c, primaryExchange );
	ADD_ATTR_STRING( c, currency );
	ADD_ATTR_STRING( c, localSymbol );
	ADD_ATTR_BOOL( c, includeExpired );
	ADD_ATTR_STRING( c, secIdType );
	ADD_ATTR_STRING( c, secId );
	ADD_ATTR_STRING( c, comboLegsDescrip );
	
	xmlAddChild(parent, ne);
}


void conv_ib2xml( xmlNodePtr parent, const IB::ContractDetails& cd,
	bool skip_defaults )
{
	char tmp[128];
	static const IB::ContractDetails dflt;
	
	xmlNodePtr ne = xmlNewChild( parent, NULL,
		(xmlChar*)"IBContractDetails", NULL);
	
	conv_ib2xml( ne, cd.summary, skip_defaults );
	
	ADD_ATTR_STRING( cd, marketName );
	ADD_ATTR_STRING( cd, tradingClass );
	ADD_ATTR_DOUBLE( cd, minTick );
	ADD_ATTR_STRING( cd, orderTypes );
	ADD_ATTR_STRING( cd, validExchanges );
	ADD_ATTR_LONG( cd, priceMagnifier );
	ADD_ATTR_INT( cd, underConId );
	ADD_ATTR_STRING( cd, longName );
	ADD_ATTR_STRING( cd, contractMonth );
	ADD_ATTR_STRING( cd, industry );
	ADD_ATTR_STRING( cd, category );
	ADD_ATTR_STRING( cd, subcategory );
	ADD_ATTR_STRING( cd, timeZoneId );
	ADD_ATTR_STRING( cd, tradingHours );
	ADD_ATTR_STRING( cd, liquidHours );
	
	// BOND values
	ADD_ATTR_STRING( cd, cusip );
	ADD_ATTR_STRING( cd, ratings );
	ADD_ATTR_STRING( cd, descAppend );
	ADD_ATTR_STRING( cd, bondType );
	ADD_ATTR_STRING( cd, couponType );
	ADD_ATTR_BOOL( cd, callable );
	ADD_ATTR_BOOL( cd, putable );
	ADD_ATTR_DOUBLE( cd, coupon );
	ADD_ATTR_BOOL( cd, convertible );
	ADD_ATTR_STRING( cd, maturity );
	ADD_ATTR_STRING( cd, issueDate );
	ADD_ATTR_STRING( cd, nextOptionDate );
	ADD_ATTR_STRING( cd, nextOptionType );
	ADD_ATTR_BOOL( cd, nextOptionPartial );
	ADD_ATTR_STRING( cd, notes );
	
	xmlAddChild(parent, ne);
}



#define GET_ATTR_INT( _struct_, _attr_ ) \
	tmp = (char*) xmlGetProp( node, (xmlChar*) #_attr_ ); \
	_struct_->_attr_ = tmp ? atoi( tmp ) : dflt._attr_; \
	free(tmp)

#define GET_ATTR_LONG( _struct_, _attr_ ) \
	tmp = (char*) xmlGetProp( node, (xmlChar*) #_attr_ ); \
	_struct_->_attr_ = tmp ? atol( tmp ) : dflt._attr_; \
	free(tmp)

#define GET_ATTR_DOUBLE( _struct_, _attr_ ) \
	tmp = (char*) xmlGetProp( node, (xmlChar*) #_attr_ ); \
	_struct_->_attr_ = tmp ? atof( tmp ) : dflt._attr_; \
	free(tmp)

#define GET_ATTR_BOOL( _struct_, _attr_ ) \
	tmp = (char*) xmlGetProp( node, (xmlChar*) #_attr_ ); \
	_struct_->_attr_ = tmp ? atoi( tmp ) : dflt._attr_; \
	free(tmp)

#define GET_ATTR_STRING( _struct_, _attr_ ) \
	tmp = (char*) xmlGetProp( node, (xmlChar*) #_attr_ ); \
	_struct_->_attr_ = tmp ? std::string(tmp) : dflt._attr_; \
	free(tmp)



void conv_xml2ib( IB::Contract* c, const xmlNodePtr node )
{
	char* tmp;
	static const IB::Contract dflt;
	
	GET_ATTR_LONG( c, conId );
	GET_ATTR_STRING( c, symbol );
	GET_ATTR_STRING( c, secType );
	GET_ATTR_STRING( c, expiry );
	GET_ATTR_DOUBLE( c, strike );
	GET_ATTR_STRING( c, right );
	GET_ATTR_STRING( c, multiplier );
	GET_ATTR_STRING( c, exchange );
	GET_ATTR_STRING( c, primaryExchange );
	GET_ATTR_STRING( c, currency );
	GET_ATTR_STRING( c, localSymbol );
	GET_ATTR_BOOL( c, includeExpired );
	GET_ATTR_STRING( c, secIdType );
	GET_ATTR_STRING( c, secId );
	GET_ATTR_STRING( c, comboLegsDescrip );
	// TODO comboLegs
	// TODO underComp
}


void conv_xml2ib( IB::ContractDetails* cd, const xmlNodePtr node )
{
	char* tmp;
	static const IB::ContractDetails dflt;
	
	xmlNodePtr xc = xmlFirstElementChild( node );
	conv_xml2ib( &cd->summary, xc );
	assert( strcmp((char*)xc->name,"IBContract") == 0 ); //TODO
	
	GET_ATTR_STRING( cd, marketName );
	GET_ATTR_STRING( cd, tradingClass );
	GET_ATTR_DOUBLE( cd, minTick );
	GET_ATTR_STRING( cd, orderTypes );
	GET_ATTR_STRING( cd, validExchanges );
	GET_ATTR_LONG( cd, priceMagnifier );
	GET_ATTR_INT( cd, underConId );
	GET_ATTR_STRING( cd, longName );
	GET_ATTR_STRING( cd, contractMonth );
	GET_ATTR_STRING( cd, industry );
	GET_ATTR_STRING( cd, category );
	GET_ATTR_STRING( cd, subcategory );
	GET_ATTR_STRING( cd, timeZoneId );
	GET_ATTR_STRING( cd, tradingHours );
	GET_ATTR_STRING( cd, liquidHours );
	
	// BOND values
	GET_ATTR_STRING( cd, cusip );
	GET_ATTR_STRING( cd, ratings );
	GET_ATTR_STRING( cd, descAppend );
	GET_ATTR_STRING( cd, bondType );
	GET_ATTR_STRING( cd, couponType );
	GET_ATTR_BOOL( cd, callable );
	GET_ATTR_BOOL( cd, putable );
	GET_ATTR_DOUBLE( cd, coupon );
	GET_ATTR_BOOL( cd, convertible );
	GET_ATTR_STRING( cd, maturity );
	GET_ATTR_STRING( cd, issueDate );
	GET_ATTR_STRING( cd, nextOptionDate );
	GET_ATTR_STRING( cd, nextOptionType );
	GET_ATTR_BOOL( cd, nextOptionPartial );
	GET_ATTR_STRING( cd, notes );
}




IbXml::IbXml()
{
	doc = xmlNewDoc( (const xmlChar*) "1.0");
	root = xmlNewDocNode( doc, NULL, (xmlChar*)"root", NULL );
	xmlDocSetRootElement( doc, root );
}

IbXml::~IbXml()
{
	xmlFreeDoc(doc);
}

void IbXml::dump() const
{
	xmlDocFormatDump(stdout, doc, 1);
}

void IbXml::add( const IB::Contract& c )
{
	conv_ib2xml( root, c );
}

void IbXml::add( const IB::ContractDetails& cd )
{
	conv_ib2xml( root, cd );
}

xmlDocPtr IbXml::getDoc() const
{
	return doc;
}

xmlNodePtr IbXml::getRoot() const
{
	return root; 
}




#ifdef TWS_XML_MAIN

int main(int argc, char *argv[])
{
	IbXml ibXml;





	IB::ContractDetails ic_orig, ic_conv;
	ic_orig.summary.strike = 25.0;
	ic_orig.coupon = 1234.567;
	ibXml.add( ic_orig );
	
	ibXml.dump();
	
	xmlNodePtr xml_contract2 = xmlFirstElementChild( ibXml.getRoot() );
	conv_xml2ib( &ic_conv, xml_contract2 );
	ic_conv.summary.strike = 27.0;
	
	#define DBG_EQUAL_FIELD( _attr_ ) \
		qDebug() << #_attr_ << ( ic_orig.summary._attr_ == ic_conv.summary._attr_ );
	DBG_EQUAL_FIELD( conId );
	DBG_EQUAL_FIELD( symbol );
	DBG_EQUAL_FIELD( secType );
	DBG_EQUAL_FIELD( expiry );
	DBG_EQUAL_FIELD( strike );
	DBG_EQUAL_FIELD( right );
	DBG_EQUAL_FIELD( multiplier );
	DBG_EQUAL_FIELD( exchange );
	DBG_EQUAL_FIELD( primaryExchange );
	DBG_EQUAL_FIELD( currency );
	DBG_EQUAL_FIELD( localSymbol );
	DBG_EQUAL_FIELD( includeExpired );
	DBG_EQUAL_FIELD( secIdType );
	DBG_EQUAL_FIELD( secId );
	DBG_EQUAL_FIELD( comboLegsDescrip );
	
	return 0;
}

#endif

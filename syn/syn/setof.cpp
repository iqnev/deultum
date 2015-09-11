#include "compiler.h"

//////////////////////////////////
//// Constructor & Destructor ////
//////////////////////////////////
template<typename inType>
setof<inType>::setof( ) {
	//for (int i=0; i<setof_len; i++) {
	//	elements[i] = false;
	//}
	reset( );
}

template<typename inType>
setof<inType>::setof( inType inArgv ) {
	//for (int i=0; i<setof_len; i++) {
	//	elements[i] = false;
	//}
	reset( );

	elements[inArgv] = true;
}

template<typename inType>
setof<inType>::setof( const int numberOfArgvs, ... ) {
	//for (int i=0; i<setof_len; i++) {
	//	elements[i] = false;
	//}
	reset( );

	va_list argvList;
	va_start( argvList, numberOfArgvs );

	for (int i=0; i<numberOfArgvs; i++ ) {
		elements[va_arg(argvList, inType)] = true;
	}
	va_end( argvList );
}

//////////////////////////
//// Public Functions ////
//////////////////////////
template<typename inType>
void setof<inType>::reset( ) {
	for (int i=0; i<setof_len; i++) {
		elements[i] = false;
		//elements[i] &= 0;
	}
}

//////////////////////////////
//// Overloaded Operators ////
//////////////////////////////
template<typename inType>
void	setof<inType>::operator += ( inType	val ) { 
	insert( val ); 
}

template<typename inType>
void	setof<inType>::operator += ( setof<inType>	val ) { 
	insert( val ); 
}

template<typename inType>
void	setof<inType>::operator -= ( inType	val ) { 
	remove( val );
}

template<typename inType>
void	setof<inType>::operator -= ( setof<inType>	val ) { 
	remove( val );
}

template<typename inType>
void	setof<inType>::operator = ( setof<inType> val )	{ 
	reset( ); 
	insert( val ); 
}

template<typename inType>
setof<inType> operator + ( setof<inType> lval, inType rval ) { 
	//return setof<inType>::rjoin( lval, setof<inType>(rval) ); 
	return lval.rjoin( lval, setof<inType>(rval) );
}

template<typename inType>
setof<inType> operator + ( inType lval, setof<inType> rval ) { 
	//return setof<inType>::rjoin( setof<inType>(lval), rval ); 
	return rval.rjoin( setof<inType>(lval), rval );
}

template<typename inType>
setof<inType> operator + ( setof<inType> lval, setof<inType> rval ) { 
	//return setof<inType>::rjoin( lval, rval ); 
	return lval.rjoin( lval, rval );
}

template<typename inType>
setof<inType> operator + ( inType lval, inType rval ) { 
	//return setof<inType>::rjoin( setof<inType>(lval), setof<inType>(rval) ); 
	return setof<inType>().rjoin( setof<inType>(lval), setof<inType>(rval ) );
}

template<typename inType>
setof<inType> operator - ( setof<inType> lval, inType rval ) {
	setof<inType> tempSet = lval;
	tempSet.remove( rval );
	return tempSet;
}

template<typename inType>
setof<inType> operator - ( setof<inType> lval, setof<inType> rval ) {
	setof<inType> tempSet = lval;
	tempSet.remove( rval );
	return tempSet;
}

template<typename inType>
bool operator >> ( inType lval, setof<inType> rval ) {
	if ( rval.elements[lval] == true ) {
		return true;
	} else { return false; }
}

template<typename inType>
bool operator >> ( setof<inType> lval, setof<inType> rval ) {
	bool	final = true;

	for (int i=0; i<setof_len; i++) {
		if ( ( lval.elements[i] == true ) && ( lval.elements[i] != rval.elements[i] ) ) {
			final = false;
			break;
		}
	}

	return final;
}

///////////////////////////
//// Private Functions ////
///////////////////////////
template<typename inType>
setof<inType> rjoin( setof<inType> inSet1, setof<inType> inSet2 ) {
	setof<inType>	tempSet;
	for (int i=0; i<setof_len; i++) {
		tempSet.elements[i] = inSet1.elements[i] | inSet2.elements[i];
	}
	return tempSet;
}

template<typename inType>
void setof<inType>::insert( const int numberOfArgvs, ... ) {
	va_list argvList;
	va_start( argvList, numberOfArgvs );

	for (int i=0; i<numberOfArgvs; i++) {
		elements[va_arg(argvList, inType)] = true;
	}

	va_end( argvList );
}
template<typename inType>
void setof<inType>::insert( inType inArgv ) {
	elements[inArgv] = true;
}
template<typename inType>
void setof<inType>::insert( setof<inType> inSet ) {
	for (int i=0; i<setof_len; i++) {
		elements[i] |= inSet.elements[i];
	}
}

template<typename inType>
void setof<inType>::remove( const int numberOfArgvs, ... ) {
	va_list argvList;
	va_start( argvList, numberOfArgvs );

	for (int i=0; i<numberOfArgvs; i++) {
		elements[va_arg(argvList, inType)] = false;
	}

	va_end( argvList );
}
template<typename inType>
void setof<inType>::remove( inType inArgv ) {
	elements[inArgv] = false;
}
template<typename inType>
void setof<inType>::remove( setof<inType> inSet ) {
	for (int i=0; i<setof_len; i++) {
		elements[i] &= inSet.elements[i];
	}
}
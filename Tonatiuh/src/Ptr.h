/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
 ***************************************************************************/
 
#ifndef PTR_H_
#define PTR_H_

template <class T>  
class Ptr 
{
public:
    Ptr( T* realPointer = 0 );
    Ptr( const Ptr& rhs );
    ~Ptr();
    Ptr& operator=( const Ptr<T>& rhs );
    Ptr& operator=( const T*& realPointer );
    T* operator->() const;
    T& operator*() const;
    operator bool() const { return m_realPointer != NULL; }
	void ShowCount() const;
	
private:
	void Init();
	T* m_realPointer;
};

template <class T>
Ptr<T>::Ptr( T* realPointer )
: m_realPointer(realPointer)
{
	Init();
}

template <class T>
Ptr<T>::Ptr( const Ptr& rhs )
: m_realPointer(rhs.m_realPointer)
{
	Init();
}

template <class T>
void Ptr<T>::Init()
{
	if( m_realPointer ) m_realPointer->Upcount();
	return;
}

template <class T>
Ptr<T>::~Ptr()
{
	if( m_realPointer ) m_realPointer->Downcount(); 
}

template <class T>
Ptr<T>& Ptr<T>::operator=( const Ptr<T>& rhs )
{
	if( m_realPointer != rhs.m_realPointer )
	{
		T* oldRealPointer = m_realPointer;
		m_realPointer = rhs.m_realPointer;
		Init();
		if( oldRealPointer ) oldRealPointer->Downcount();
	}
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=( const T*& realPointer )
{
	if( m_realPointer != realPointer )
	{
		T* oldRealPointer = m_realPointer;
		m_realPointer = realPointer;
		Init();
		if( oldRealPointer ) oldRealPointer->Downcount();
	}
	return *this;
}

template <class T>
T* Ptr<T>::operator->() const
{
	return m_realPointer;
}

template <class T>
T& Ptr<T>::operator*() const
{
	return *m_realPointer;
}

template <class T>
void Ptr<T>::ShowCount() const
{ 
	m_realPointer->PrintCount();
}

#endif /*PTR_H_*/
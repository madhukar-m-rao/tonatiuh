/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "CmdDeleteTracker.h"
#include "InstanceNode.h"
#include "SceneModel.h"
#include "tgf.h"
#include "TLightKit.h"
#include "Trace.h"
#include "TSeparatorKit.h"
#include "TTracker.h"


CmdDeleteTracker::CmdDeleteTracker( const QModelIndex& selectedIndex, SoSceneKit* scene, SceneModel& model, QUndoCommand* parent )
: QUndoCommand("Delete", parent), m_tracker( 0 ),  m_coinParent( 0 ),  m_scene( scene ), m_pModel( &model ), m_row( 0 )
{
	Trace trace( "CmdDeleteTracker::CmdDeleteTracker", false );

	if( !m_scene->getPart("lightList[0]", false) )	 tgf::SevereError( "CmdDeleteTracker Null lightKit." );

	if( !selectedIndex.isValid() ) tgf::SevereError( "CmdDeleteTracker called with invalid ModelIndex." );
	InstanceNode* instanceSelection = m_pModel->NodeFromIndex( selectedIndex );

	if( !instanceSelection->GetNode() ) tgf::SevereError( "CmdDeleteTracker called with NULL selection node." );
	m_coinParent = static_cast< TSeparatorKit* > ( instanceSelection->GetParent()->GetNode() );
	if( !m_coinParent ) tgf::SevereError( "CmdDeleteTracker called with invalid tracker parent." );

	m_tracker = dynamic_cast<TTracker* > ( m_coinParent->getPart( "tracker", false) );
	if( !m_tracker ) tgf::SevereError( "CmdDeleteTracker Null tracker." );
	m_tracker->ref();

	m_row = instanceSelection->GetParent()->children.indexOf( instanceSelection );

}

CmdDeleteTracker::~CmdDeleteTracker()
{
	Trace trace( "CmdDeleteTracker::~CmdDeleteTracker", false );

	m_tracker->unref();
}

void CmdDeleteTracker::undo()
{
	Trace trace( "CmdDeleteTracker::undo", false );

	TLightKit* lightKit = static_cast< TLightKit* >( m_scene->getPart("lightList[0]", false) );

	SoTransform* parentTransform = static_cast< SoTransform* > ( m_coinParent->getPart("transform", true ) );
	if( !parentTransform ) tgf::SevereError( "CmdInsertTracker Null node transform." );

	m_tracker->SetAzimuthAngle( &lightKit->azimuth );
	m_tracker->SetZenithAngle( lightKit->zenith );
	parentTransform->rotation.connectFrom( &m_tracker->outputRotation );

	m_pModel->Paste( tgc::Shared, *m_coinParent, *m_tracker, m_row );
}

void CmdDeleteTracker::redo( )
{
	Trace trace( "CmdDeleteTracker::redo", true );

	SoTransform* transform = static_cast< SoTransform* > ( m_coinParent->getPart("transform", true ) );
	transform->rotation.disconnect();
	m_tracker->Disconnect();

	m_pModel->Cut( *m_coinParent, m_row );

}

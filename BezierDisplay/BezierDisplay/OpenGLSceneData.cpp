//
//  OpenGLSceneData.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 12/8/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "OpenGLSceneData.h"

SceneObjectData::SceneObjectData()
{};

SceneControllerData::SceneControllerData()
: m_left_drag( false )
, m_right_drag( false )
, m_activeshift( false )
, m_last_x( 0 )
, m_last_y( 0 )
, m_recenterScale (1.0)
{};

SceneControllerData::SceneControllerData( const scalar & rescale )
: m_left_drag( false )
, m_right_drag( false )
, m_activeshift( false )
, m_last_x( 0 )
, m_last_y( 0 )
, m_recenterScale (rescale)
{};

SceneMiscData::SceneMiscData()
: m_bgColor( 1.0, 1.0, 1.0 )
{};

SceneMiscData::SceneMiscData( const renderingutils::Color & bg )
: m_bgColor( bg )
{};
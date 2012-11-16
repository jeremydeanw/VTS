//
//  SVGSceneParser.h
//  VTS
//
//  Created by Papoj Thamjaroenporn on 10/11/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __VTS__SVGSceneParser__
#define __VTS__SVGSceneParser__

#include "rapidxml.hpp"
#include <Eigen/StdVector>

#include <iostream>
#include <string>
#include <vector>

#include "ParserUtils.h"
#include "BezierCurve.h"








#include <iostream>
#include <fstream>
#include <limits>

#include "rapidxml.hpp"

#include "TwoDScene.h"

#include "ExplicitEuler.h"
#include "SemiImplicitEuler.h"
#include "ImplicitEuler.h"
#include "LinearizedImplicitEuler.h"

#include "SpringForce.h"
#include "GravitationalForce.h"
#include "ConstantForce.h"
#include "SimpleGravityForce.h"
#include "DragDampingForce.h"
#include "VortexForce.h"

#include "StringUtilities.h"
#include "RenderingUtilities.h"
#include "CollisionHandler.h"

#include "SimpleCollisionHandler.h"
#include "ContinuousTimeCollisionHandler.h"
#include "PenaltyForce.h"

#include "TwoDSceneRenderer.h"
#include "TwoDSceneSVGRenderer.h"
#include "ExecutableSimulation.h"
#include "TwoDimensionalDisplayController.h"
#include "ParticleSimulation.h"

typedef std::vector< std::string > TokenList;

class SVGSceneParser
{
public:
	void loadSVGScene( const std::string& file_name, std::string& description, std::string &title );

private:

	/////////////////////////////////////////////////////////////////////////////
	// General helpers
	void loadXMLFile( const std::string& filename,
						std::vector<char>& xmlchars,
						rapidxml::xml_document<>& doc );
				
	bool loadTextFileIntoString( const std::string& filename, std::string& filecontents );
	
	/////////////////////////////////////////////////////////////////////////////
	// General component loaders
	void loadSceneDescriptionString( rapidxml::xml_node<>* node, std::string& description_string );
	
	void loadSceneTitleString( rapidxml::xml_node<>* node, std::string& title_string );
	
	/////////////////////////////////////////////////////////////////////////////
	// Main components loaders
	void loadPaths( rapidxml::xml_node<>* node, PolyBezierCurve &pbc );
	
	bool parsePath( const std::vector< std::string > &tokens, PolyBezierCurve &pbc );
	

  void loadParticleSimulation( bool simulate_comparison, bool rendering_enabled, TwoDimensionalDisplayController& display_controller, ExecutableSimulation** execsim, 
                               renderingutils::Viewport& view, scalar& dt, renderingutils::Color& bgcolor, rapidxml::xml_node<>* node );

  void loadRigidBodySimulation( bool simulate_comparison, bool rendering_enabled, TwoDimensionalDisplayController& display_controller, ExecutableSimulation** execsim, renderingutils::Viewport& view, scalar& dt, renderingutils::Color& bgcolor, rapidxml::xml_node<>* node );

  
  void loadSimulationType( rapidxml::xml_node<>* node, std::string& simtype );

  /////////////////////////////////////////////////////////////////////////////
  // Camera parsing

  void loadViewport( rapidxml::xml_node<> *node, renderingutils::Viewport &view);

  
  void loadParticles( rapidxml::xml_node<>* node, TwoDScene& twodscene );

  void loadSceneTag( rapidxml::xml_node<>* node, std::string& scenetag );
  
  void loadEdges( rapidxml::xml_node<>* node, TwoDScene& twodscene );

  void loadHalfplanes( rapidxml::xml_node<> *node, TwoDScene &twoscene);

  void loadSpringForces( rapidxml::xml_node<>* node, TwoDScene& twodscene );

  void loadGravitationalForces( rapidxml::xml_node<>* node, TwoDScene& twodscene );
  
  void loadSimpleGravityForces( rapidxml::xml_node<>* node, TwoDScene& twodscene );
  
  void loadConstantForces( rapidxml::xml_node<>* node, TwoDScene& twodscene );

  void loadDragDampingForces( rapidxml::xml_node<>* node, TwoDScene& twodscene );

  void loadVorexForces( rapidxml::xml_node<>* node, TwoDScene& twodscene );

  void loadIntegrator( rapidxml::xml_node<>* node, SceneStepper** scenestepper, scalar& dt );

  void loadCollisionHandler( rapidxml::xml_node<>* node, TwoDScene &scene, CollisionHandler **handler, CollisionDetector &detector);

  void loadCollisionDetector( rapidxml::xml_node<> *node, TwoDScene &scene, CollisionDetector **handler);
  
  void loadMaxTime( rapidxml::xml_node<>* node, scalar& max_t );
  
  void loadMaxSimFrequency( rapidxml::xml_node<>* node, scalar& max_freq );
  
  void loadBackgroundColor( rapidxml::xml_node<>* node, renderingutils::Color& color );
  
  void loadParticleColors( rapidxml::xml_node<>* node, std::vector<renderingutils::Color>& particle_colors );

  void loadEdgeColors( rapidxml::xml_node<>* node, std::vector<renderingutils::Color>& edge_colors );

  void loadHalfplaneColors( rapidxml::xml_node<> *node, std::vector<renderingutils::Color> &halfplane_colors );
  
  void loadParticlePaths( rapidxml::xml_node<>* node, double dt, std::vector<renderingutils::ParticlePath>& particle_paths );

};

#endif /* defined(__VTS__SVGSceneParser__) */

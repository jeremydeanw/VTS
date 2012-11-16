//
//  SVGSceneParser.cpp
//  VTS
//
//  Created by Papoj Thamjaroenporn on 10/11/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include <regex>

#include "SVGSceneParser.h"

#include "SVGSceneParser.h"
#include "HybridCollisionHandler.h"
#include "CollisionDetector.h"
#include "AllPairsDetector.h"
#include "ContestDetector.h"


void SVGSceneParser::loadSVGScene(const std::string &file_name, std::string &description, std::string &title)
{
	// Load the xml document
	std::vector<char> xmlchars;
	rapidxml::xml_document<> doc;
	loadXMLFile( file_name, xmlchars, doc );

	// Attempt to locate the root node
	rapidxml::xml_node<>* node = doc.first_node("svg");
	if( node == NULL ) 
	{
		std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse svg file. Failed to locate root <svg> node. Exiting." << std::endl;
		exit(1);
	}

	// Parse common state
	//loadMaxTime( node, max_time );
	//loadMaxSimFrequency( node, steps_per_sec_cap );
	//loadViewport( node, view );
	//loadBackgroundColor( node, bgcolor );
	loadSceneDescriptionString( node, description );
	loadSceneTitleString( node, title );
	//loadSceneTag( node, scenetag );

	loadParticleSimulation( simulate_comparison, rendering_enabled, display_controller, execsim, view, dt, bgcolor, node );
}

/////////////////////////////////////////////////////////////////////////////
//
// General helpers
//
/////////////////////////////////////////////////////////////////////////////


void SVGSceneParser::loadXMLFile( const std::string& filename,
									std::vector<char>& xmlchars,
									rapidxml::xml_document<>& doc )
{
	// Attempt to read the text from the user-specified xml file
	std::string filecontents;
	if( !loadTextFileIntoString(filename,filecontents) )
	{
	std::cerr << "\033[31;1mERROR IN SVGSceneParser:\033[m XML scene file " << filename << ". Failed to read file." << std::endl;
	exit(1);
	}

	// Copy string into an array of characters for the xml parser
	for( int i = 0; i < (int) filecontents.size(); ++i ) xmlchars.push_back(filecontents[i]);
	xmlchars.push_back('\0');

	// Initialize the xml parser with the character vector
	doc.parse<0>(&xmlchars[0]);
}

bool SVGSceneParser::loadTextFileIntoString( const std::string& filename, std::string& filecontents )
{
	// Attempt to open the text file for reading
	std::ifstream textfile(filename.c_str(),std::ifstream::in);
	if(!textfile) return false;

	// Read the entire file into a single string
	std::string line;
	while(getline(textfile,line)) filecontents.append(line);

	textfile.close();

	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// General component loaders
//
/////////////////////////////////////////////////////////////////////////////

void SVGSceneParser::loadSceneDescriptionString( rapidxml::xml_node<>* node, std::string& description_string )
{
	assert( node != NULL );

	description_string = "No description specified.";

	// Attempt to locate the desc node
	rapidxml::xml_node<>* nd = node->first_node("desc");
	if( nd != NULL ) 
	{
		description_string = typend->value();
	}
}

void SVGSceneParser::loadSceneTitleString( rapidxml::xml_node<>* node, std::string& title_string )
{
	assert( node != NULL );
	
	title_string = "No title specified.";
	
	// Attempt to locate the title node
	rapidxml::xml_node<>* nd = node->first_node("title");
	if( nd != NULL )
	{
		title_string = nd->value();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// Main components loaders
//
/////////////////////////////////////////////////////////////////////////////

void loadPaths( rapidxml::xml_node<>* node, PolyBezierCurve &pbc )
{
	assert( node != NULL );

	// CLEAR BIZIER POLY OBJECT
	
	int pathIndex = 0;
	std::string path_data;
	for( rapidxml::xml_node<>* nd = node->first_node("path"); nd; nd = nd->next_sibling("path") )
	{
		if( nd->first_attribute("d") )
		{
			path_data = nd->first_attribute("d")->value();
			// NOW extract the data points from path data!
			// PUT THEM AS SET OF CONTROL POINTS WITHIN POLYBIZIER
			cout << "path data: " << path_data << endl;
			
			TokenList tokens;
			tokenizeBySpaces( path_data, tokens );
			if( !parsePath( tokens, pbc ) )
			{
				std::cerr << "\033[31;1mERROR IN SVGSceneParser:\033[m XML scene file " << filename << ". Error in definition of curve path, id: " << nd->first_attribute("id")->value() << std::endl;
				exit(1);
			}
		}
	}

	// Attempt to locate the path node
	rapidxml::xml_node<>* nd = node->first_node("path");
}

// Extract the control points from the path given by token list.
// See SVG Specification for Paths for more information.
//
// TODO: This parsing currently ignores the case where the command and numbers are not separated
// by whitespaces.
// TODO: Does not yet handle anomaly like: 0 1,100 (where the syntax should look like 0,3 1,100).
bool parsePath( const std::vector< std::string > &tokens, PolyBezierCurve &pbc )
{
	
	Vector2s * currentPointPtr;
	Vector2s * prevPointPtr;
	Vector2s currentPoint;
	Vector2s prevPoint;
	
	unsigned int size = tokens.size();
	
	std::vector< std::string > commaTokens;
	scalar currentX; bool isXFilled;
	std::string currentToken;
	unsigned int i = 0;
	while( i < size )
	{
		commaTokens.clear();
		isXFilled = false;
		currentToken = tokens[i];
		
		// Every time command moveto is called, we disregard all previous points.
		if( currentToken == "M" || currentToken == "m")
		{
			while ( std::regex_match( token[i+1], std::regex("\\w")) )
			{
				++i;
				
				// Extract x,y pair(s). The syntax can either be (x y) or (x,y)
				tokenizeByComma( token[i], commaTokens );
				if( commaTokens.size() < 2) {
					if ( isXFilled )
					{
						prevPoint = currentPoint;
						currentPoint = Vector2s( currentX, commaTokens[0] );
						isXFilled = false;
					}
					else
					{
						currentX = commaTokens[0];
						isXFilled = true;
					}
				}
				else
				{
					prevPoint = currentPoint;
					currentPoint = Vector2s( commaTokens[0], commaTokens[1] );
				}
			}
			
		
//			// Extract x,y pair(s)
//			if ( i+1 == size || ( i+2 == size && ) )
//			
//			void tokenizeBySpace( std::string &str, std::vector< std::string > &tokens );
			
		}
		i++;
	}

	return true;
}


void SVGSceneParser::loadParticleSimulation( bool simulate_comparison, bool rendering_enabled, TwoDimensionalDisplayController& display_controller, ExecutableSimulation** execsim, renderingutils::Viewport& view, scalar& dt, renderingutils::Color& bgcolor, rapidxml::xml_node<>* node )
{
  TwoDScene* scene = new TwoDScene;

  // Scene
  loadParticles( node, *scene );
  loadEdges( node, *scene );
  loadHalfplanes( node, *scene );

  // Forces
  loadSpringForces( node, *scene );
  loadSimpleGravityForces( node, *scene );
  loadGravitationalForces( node, *scene );
  loadConstantForces( node, *scene );
  loadDragDampingForces( node, *scene );
  loadVorexForces( node, *scene );

  // Integrator/solver
  SceneStepper* scene_stepper = NULL;
  loadIntegrator( node, &scene_stepper, dt );
  assert( scene_stepper != NULL );
  assert( dt > 0.0 );

  CollisionDetector *collision_detector = NULL;
  loadCollisionDetector(node, *scene, &collision_detector);
  assert(collision_detector != NULL);
  CollisionHandler* collision_handler = NULL;
  loadCollisionHandler(node, *scene, &collision_handler, *collision_detector);


  // Rendering state
  std::vector<renderingutils::Color> particle_colors;
  particle_colors.resize(scene->getNumParticles(),renderingutils::Color(0.650980392156863,0.294117647058824,0.0));
  loadParticleColors( node, particle_colors );

  std::vector<renderingutils::Color> edge_colors;
  edge_colors.resize(scene->getNumEdges(),renderingutils::Color(0.0,0.388235294117647,0.388235294117647));
  loadEdgeColors( node, edge_colors );

  std::vector<renderingutils::Color> halfplane_colors;
  halfplane_colors.resize(scene->getNumHalfplanes(), renderingutils::Color(0,0,0));
  loadHalfplaneColors( node, halfplane_colors );

  std::vector<renderingutils::ParticlePath> particle_paths;
  loadParticlePaths( node, dt, particle_paths );  
  
  
  TwoDScene* comparison_scene = NULL;
  if( simulate_comparison )
  {
    comparison_scene = new TwoDScene;
    comparison_scene->copyState(*scene);
  }
  
  TwoDSceneRenderer* scene_renderer = NULL;
  if( rendering_enabled )
  {
    scene_renderer = new TwoDSceneRenderer(*scene,display_controller,particle_colors,edge_colors,halfplane_colors,particle_paths);
    scene_renderer->updateParticleSimulationState(*scene);
  }
  
  TwoDSceneSVGRenderer* svg_renderer = new TwoDSceneSVGRenderer(*scene,display_controller,particle_colors,edge_colors,halfplane_colors,particle_paths,512,512,bgcolor);
  svg_renderer->updateState();
  
  *execsim = new ParticleSimulation(scene, comparison_scene, collision_detector, collision_handler, scene_stepper, scene_renderer, svg_renderer);
}

void SVGSceneParser::loadSimulationType( rapidxml::xml_node<>* node, std::string& simtype )
{
  assert( node != NULL );
  rapidxml::xml_node<>* nd = node->first_node("simtype");
  
  if( node->first_node("simtype") ) if( node->first_node("simtype")->first_attribute("type") ) simtype = node->first_node("simtype")->first_attribute("type")->value();
}

void SVGSceneParser::loadParticles( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );
  
  // Count the number of particles
  int numparticles = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("particle"); nd; nd = nd->next_sibling("particle") ) ++numparticles;
  
  twodscene.resizeSystem(numparticles);
  
  //std::cout << "Num particles " << numparticles << std::endl;
  
  std::vector<std::string>& tags = twodscene.getParticleTags();
  
  int particle = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("particle"); nd; nd = nd->next_sibling("particle") )
  {
    // Extract the particle's initial position
    Vector2s pos;
    if( nd->first_attribute("px") ) 
    {
      std::string attribute(nd->first_attribute("px")->value());
      if( !stringutils::extractFromString(attribute,pos.x()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of px attribute for particle " << particle << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse px attribute for particle " << particle << ". Exiting." << std::endl;
      exit(1);
    }

    if( nd->first_attribute("py") ) 
    {
      std::string attribute(nd->first_attribute("py")->value());
      if( !stringutils::extractFromString(attribute,pos.y()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of py attribute for particle " << particle << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse py attribute for particle " << particle << ". Exiting." << std::endl;
      exit(1);
    }
    twodscene.setPosition( particle, pos );

    // Extract the particle's initial velocity
    Vector2s vel;
    if( nd->first_attribute("vx") ) 
    {
      std::string attribute(nd->first_attribute("vx")->value());
      if( !stringutils::extractFromString(attribute,vel.x()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of vx attribute for particle " << particle << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse vx attribute for particle " << particle << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( nd->first_attribute("vy") ) 
    {
      std::string attribute(nd->first_attribute("vy")->value());
      if( !stringutils::extractFromString(attribute,vel.y()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of vy attribute for particle " << particle << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse vy attribute for particle " << particle << ". Exiting." << std::endl;
      exit(1);
    }
    twodscene.setVelocity( particle, vel );

    // Extract the particle's mass
    scalar mass = std::numeric_limits<scalar>::signaling_NaN();
    if( nd->first_attribute("m") ) 
    {
      std::string attribute(nd->first_attribute("m")->value());
      if( !stringutils::extractFromString(attribute,mass) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of m attribute for particle " << particle << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse m attribute for particle " << particle << ". Exiting." << std::endl;
      exit(1);
    }
    twodscene.setMass( particle, mass );
    
    // Determine if the particle is fixed
    bool fixed;
    if( nd->first_attribute("fixed") ) 
    {
      std::string attribute(nd->first_attribute("fixed")->value());
      if( !stringutils::extractFromString(attribute,fixed) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of fixed attribute for particle " << particle << ". Value must be boolean. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse fixed attribute for particle " << particle << ". Exiting." << std::endl;
      exit(1);
    }
    twodscene.setFixed( particle, fixed );

    // Extract the particle's radius, if present
    scalar radius = 0.1;
    if( nd->first_attribute("radius") )
    {
      std::string attribute(nd->first_attribute("radius")->value());
      if( !stringutils::extractFromString(attribute,radius) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse radius attribute for particle " << particle << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }
    }    
    twodscene.setRadius( particle, radius );

    // Extract the particle's tag, if present
    if( nd->first_attribute("tag") )
    {
      std::string tag(nd->first_attribute("tag")->value());
      tags[particle] = tag;
    }    

    //std::cout << "Particle: " << particle << "    x: " << pos.transpose() << "   v: " << vel.transpose() << "   m: " << mass << "   fixed: " << fixed << std::endl;
    //std::cout << tags[particle] << std::endl;
    
    ++particle;
  }
}

void SVGSceneParser::loadSceneTag( rapidxml::xml_node<>* node, std::string& scenetag )
{
  assert( node != NULL );

  if( node->first_node("scenetag") )
  {
    if( node->first_node("scenetag")->first_attribute("tag") )
    {
      scenetag = node->first_node("scenetag")->first_attribute("tag")->value();
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of tag attribute for scenetag. Value must be string. Exiting." << std::endl;
      exit(1);
    }
  }
}

void SVGSceneParser::loadHalfplanes( rapidxml::xml_node<> *node, TwoDScene &twodscene)
{
  assert( node != NULL );

  twodscene.clearHalfplanes();

  int halfplane = 0;
  for( rapidxml::xml_node<> *nd = node->first_node("halfplane"); nd; nd = nd->next_sibling("halfplane") )
    {
      VectorXs x(2);
      VectorXs n(2);
      
      if( nd->first_attribute("px") )
	{
	  std::string attribute(nd->first_attribute("px")->value());
	  if( !stringutils::extractFromString(attribute, x[0]) )
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of px attribute for halfplane " << halfplane << ". Value must be real number. Exiting." << std::endl;
	      exit(0);
	    }
	}
      else
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of px attribute for halfplane " << halfplane << ". Exiting." << std::endl;
	  exit(0);
	}
      if( nd->first_attribute("py") )
	{
	  std::string attribute(nd->first_attribute("py")->value());
	  if( !stringutils::extractFromString(attribute, x[1]) )
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of py attribute for halfplane " << halfplane << ". Value must be real number. Exiting." << std::endl;
	      exit(0);
	    }
	}
      else
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of py attribute for halfplane " << halfplane << ". Exiting." << std::endl;
	  exit(0);
	}
      if( nd->first_attribute("nx") )
	{
	  std::string attribute(nd->first_attribute("nx")->value());
	  if( !stringutils::extractFromString(attribute, n[0]) )
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of nx attribute for halfplane " << halfplane << ". Value must be real number. Exiting." << std::endl;
	      exit(0);
	    }
	}
      else
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of nx attribute for halfplane " << halfplane << ". Exiting." << std::endl;
	  exit(0);
	}
      if( nd->first_attribute("ny") )
	{
	  std::string attribute(nd->first_attribute("ny")->value());
	  if( !stringutils::extractFromString(attribute, n[1]) )
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of ny attribute for halfplane " << halfplane << ". Value must be real number. Exiting." << std::endl;
	      exit(0);
	    }
	}
      else
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of ny attribute for halfplane " << halfplane << ". Exiting." << std::endl;
	  exit(0);
	}
      twodscene.insertHalfplane(std::pair<VectorXs, VectorXs>(x,n));

      ++halfplane;
    }
}

void SVGSceneParser::loadEdges( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );

  twodscene.clearEdges();
  
  int edge = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("edge"); nd; nd = nd->next_sibling("edge") )
  {
    std::pair<int,int> newedge;
    if( nd->first_attribute("i") )
    {
      std::string attribute(nd->first_attribute("i")->value());
      if( !stringutils::extractFromString(attribute,newedge.first) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for edge " << edge << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for edge " << edge << ". Exiting." << std::endl;
      exit(1);
    }

    if( nd->first_attribute("j") )
    {
      std::string attribute(nd->first_attribute("j")->value());
      if( !stringutils::extractFromString(attribute,newedge.second) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of j attribute for edge " << edge << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of j attribute for edge " << edge << ". Exiting." << std::endl;
      exit(1);
    }
    
    scalar radius = 0.015;
    if( nd->first_attribute("radius") )
    {
      std::string attribute(nd->first_attribute("radius")->value());
      if( !stringutils::extractFromString(attribute,radius) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse radius attribute for edge " << edge << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }
    }

    //std::cout << "Edge: " << edge << "    i: " << newedge.first << "   j: " << newedge.second << std::endl;
    
    twodscene.insertEdge( newedge, radius );
    
    ++edge;
  }
}

void SVGSceneParser::loadSpringForces( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );
  
  // Extract the edge the force acts across
  int forcenum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("springforce"); nd; nd = nd->next_sibling("springforce") )
  {
    int edge = -1;

    if( nd->first_attribute("edge") )
    {
      std::string attribute(nd->first_attribute("edge")->value());
      if( !stringutils::extractFromString(attribute,edge) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of edge attribute for springforce " << forcenum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of edge attribute for springforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }      
  
    std::pair<int,int> newedge(twodscene.getEdge(edge));
    
    // Extract the spring stiffness
    scalar k = std::numeric_limits<scalar>::signaling_NaN();
    if( nd->first_attribute("k") ) 
    {
      std::string attribute(nd->first_attribute("k")->value());
      if( !stringutils::extractFromString(attribute,k) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of k attribute for springforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse k attribute for springforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    // Extract the spring rest length
    scalar l0 = std::numeric_limits<scalar>::signaling_NaN();
    if( nd->first_attribute("l0") ) 
    {
      std::string attribute(nd->first_attribute("l0")->value());
      if( !stringutils::extractFromString(attribute,l0) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of l0 attribute for springforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse l0 attribute for springforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }

    // Extract the optional damping coefficient
    scalar b = 0.0;
    if( nd->first_attribute("b") ) 
    {
      std::string attribute(nd->first_attribute("b")->value());
      if( !stringutils::extractFromString(attribute,b) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for springforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    
    //std::cout << "Springforce: " << forcenum << "    i: " << newedge.first << "   j: " << newedge.second << "   k: " << k << "   l0: " << l0 << std::endl;
    
    twodscene.insertForce(new SpringForce(newedge,k,l0,b));
    
    ++forcenum;
  }

  //SpringForce( const std::pair<int,int>& endpoints, const scalar& k, const scalar& l0 )
}

void SVGSceneParser::loadGravitationalForces( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );
  
  // Extract the edge the force acts across
  int forcenum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("gravitationalforce"); nd; nd = nd->next_sibling("gravitationalforce") )
  {
    std::pair<int,int> newedge;
    if( nd->first_attribute("i") )
    {
      std::string attribute(nd->first_attribute("i")->value());
      if( !stringutils::extractFromString(attribute,newedge.first) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for gravitationalforce " << forcenum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for gravitationalforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( nd->first_attribute("j") )
    {
      std::string attribute(nd->first_attribute("j")->value());
      if( !stringutils::extractFromString(attribute,newedge.second) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of j attribute for gravitationalforce " << forcenum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of j attribute for gravitationalforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    // Extract the gravitational constant
    scalar G = std::numeric_limits<scalar>::signaling_NaN();
    if( nd->first_attribute("G") )
    {
      std::string attribute(nd->first_attribute("G")->value());
      if( !stringutils::extractFromString(attribute,G) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of G attribute for gravitationalforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse G attribute for gravitationalforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }

    //std::cout << "GravitationalForce: " << forcenum << "    i: " << newedge.first << "   j: " << newedge.second << "   G: " << G << std::endl;

    twodscene.insertForce(new GravitationalForce(newedge,G));
    
    ++forcenum;
  }
  
  //SpringForce( const std::pair<int,int>& endpoints, const scalar& k, const scalar& l0 )  
}


void SVGSceneParser::loadConstantForces( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );

  // Load each constant force
  int forcenum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("constantforce"); nd; nd = nd->next_sibling("constantforce") )
  {
    Vector2s constforce;
    constforce.setConstant(std::numeric_limits<scalar>::signaling_NaN());
    
    // Extract the x component of the force
    if( nd->first_attribute("fx") ) 
    {
      std::string attribute(nd->first_attribute("fx")->value());
      if( !stringutils::extractFromString(attribute,constforce.x()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of fx attribute for constantforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse fx attribute for constantforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }

    // Extract the y component of the force
    if( nd->first_attribute("fy") ) 
    {
      std::string attribute(nd->first_attribute("fy")->value());
      if( !stringutils::extractFromString(attribute,constforce.y()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of fy attribute for constantforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse fy attribute for constantforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }

    //std::cout << "x: " << constforce.transpose() << std::endl;

    twodscene.insertForce(new ConstantForce(constforce));

    ++forcenum;
  }
}

void SVGSceneParser::loadDragDampingForces( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );
  
  int forcenum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("dragdamping"); nd; nd = nd->next_sibling("dragdamping") )
  {
    Vector2s constforce;
    constforce.setConstant(std::numeric_limits<scalar>::signaling_NaN());
    
    // Extract the linear damping coefficient
    scalar b = std::numeric_limits<scalar>::signaling_NaN();
    if( nd->first_attribute("b") )
    {
      std::string attribute(nd->first_attribute("b")->value());
      if( !stringutils::extractFromString(attribute,b) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for dragdamping " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse b attribute for dragdamping " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    //std::cout << "x: " << constforce.transpose() << std::endl;
    
    twodscene.insertForce(new DragDampingForce(b));
    
    ++forcenum;
  }  
}

void SVGSceneParser::loadVorexForces( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );
  
  // Extract the edge the force acts across
  int forcenum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("vortexforce"); nd; nd = nd->next_sibling("vortexforce") )
  {
    std::pair<int,int> newedge;
    if( nd->first_attribute("i") )
    {
      std::string attribute(nd->first_attribute("i")->value());
      if( !stringutils::extractFromString(attribute,newedge.first) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for vortexforce " << forcenum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for vortexforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( nd->first_attribute("j") )
    {
      std::string attribute(nd->first_attribute("j")->value());
      if( !stringutils::extractFromString(attribute,newedge.second) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of j attribute for vortexforce " << forcenum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of j attribute for vortexforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    // Extract the 'Biot-Savart' constant
    scalar kbs = std::numeric_limits<scalar>::signaling_NaN();
    if( nd->first_attribute("kbs") )
    {
      std::string attribute(nd->first_attribute("kbs")->value());
      if( !stringutils::extractFromString(attribute,kbs) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of kbs attribute for vortexforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse kbs attribute for vortexforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    // Extract the viscosity constant
    scalar kvc = std::numeric_limits<scalar>::signaling_NaN();
    if( nd->first_attribute("kvc") )
    {
      std::string attribute(nd->first_attribute("kvc")->value());
      if( !stringutils::extractFromString(attribute,kvc) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of kvc attribute for vortexforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse kvc attribute for vortexforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }    
    
    //std::cout << "VortexForce: " << forcenum << "    i: " << newedge.first << "   j: " << newedge.second << "   kbs: " << kbs << "   kvc: " << kvc << std::endl;
    
    twodscene.insertForce(new VortexForce(newedge,kbs,kvc));
    
    ++forcenum;
  }
}

void SVGSceneParser::loadSimpleGravityForces( rapidxml::xml_node<>* node, TwoDScene& twodscene )
{
  assert( node != NULL );
  
  // Load each constant force
  int forcenum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("simplegravity"); nd; nd = nd->next_sibling("simplegravity") )
  {
    Vector2s constforce;
    constforce.setConstant(std::numeric_limits<scalar>::signaling_NaN());
    
    // Extract the x component of the force
    if( nd->first_attribute("fx") ) 
    {
      std::string attribute(nd->first_attribute("fx")->value());
      if( !stringutils::extractFromString(attribute,constforce.x()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of fx attribute for constantforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse fx attribute for constantforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    // Extract the y component of the force
    if( nd->first_attribute("fy") ) 
    {
      std::string attribute(nd->first_attribute("fy")->value());
      if( !stringutils::extractFromString(attribute,constforce.y()) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of fy attribute for constantforce " << forcenum << ". Value must be numeric. Exiting." << std::endl;
        exit(1);
      }
    }
    else 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse fy attribute for constantforce " << forcenum << ". Exiting." << std::endl;
      exit(1);
    }
    
    //std::cout << "x: " << constforce.transpose() << std::endl;
    
    twodscene.insertForce(new SimpleGravityForce(constforce));
    
    ++forcenum;
  }  
}

void SVGSceneParser::loadCollisionDetector( rapidxml::xml_node<> *node, TwoDScene &scene, CollisionDetector **detector )
{
  assert(node != NULL);

  rapidxml::xml_node<> *nd = node->first_node("collisiondetection");
  if( nd == NULL )
    {
      *detector = new AllPairsDetector();
      return;
    }

  // Load value of type
  rapidxml::xml_attribute<> *typend = nd->first_attribute("type");
  if(typend == NULL)
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse type attribute for collisiondetector node. Exiting." << std::endl;
      exit(1);
    }
  std::string type = typend->value();
  if(type == "allpairs")
    *detector = new AllPairsDetector();
  else if(type == "contest")
    *detector = new ContestDetector();
  else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Invalid type attribute in collisiondetector node. Exiting." << std::endl;
      exit(1);
    }
}

void SVGSceneParser::loadCollisionHandler( rapidxml::xml_node<>* node, TwoDScene &scene, CollisionHandler **handler, CollisionDetector &detector )
{
  assert( node != NULL );

  rapidxml::xml_node<>* nd = node->first_node("collision");
  if( nd == NULL )
    {
      *handler = NULL;
      return;
    }

  // Load value of COR
  double cor = 1.0;
  rapidxml::xml_attribute<> *cornd = nd->first_attribute("COR");
  if( cornd != NULL )
    { 
      if( !stringutils::extractFromString(std::string(cornd->value()),cor) )
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'COR' attribute for collision handler. Value must be numeric. Exiting." << std::endl;
	  exit(1);
	}
    }
  
  // Attempt to load the collision handler type
  rapidxml::xml_attribute<> *typend = nd->first_attribute("type");
  if( typend == NULL )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No collision handler 'type' attribute specified. Exiting." << std::endl;
      exit(1);
    }
  std::string handlertype(typend->value());
  if(handlertype == "none") *handler = NULL;
  else if(handlertype == "simple") *handler = new SimpleCollisionHandler(cor);
  else if(handlertype == "continuous-time") *handler = new ContinuousTimeCollisionHandler(cor);
  else if(handlertype == "hybrid")
    {
      // Load max iteration attribute
      double maxiters=10;
      rapidxml::xml_attribute<> *maxiternd = nd->first_attribute("maxiters");
      if(maxiternd != NULL)
	{
	  if(!stringutils::extractFromString(std::string(maxiternd->value()), maxiters))
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'maxiters' attribute for collision handler. Value must be numeric. Exiting."<< std::endl;
	      exit(1);
	    }
	}
      *handler = new HybridCollisionHandler(maxiters, cor);

      // Attempt to load stiffness attribute
      double k=10;
      rapidxml::xml_attribute<> *knd = nd->first_attribute("k");
      if(knd != NULL)
	{
	  if(!stringutils::extractFromString(std::string(knd->value()), k))
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'k' attribute for collision handler. Value must be numeric. Exiting." << std::endl;
	      exit(1);
	    }
	}

      double thickness=0.1;
      rapidxml::xml_attribute<> *thicknd = nd->first_attribute("thickness");
      if(thicknd != NULL)
	{
	  if(!stringutils::extractFromString(std::string(thicknd->value()), thickness))
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'thickness' attribute for collision handler. Value must be numeric. Exiting." << std::endl;
	      exit(1);
	    }
	}
      scene.insertForce(new PenaltyForce(scene, detector, k, thickness));
    }
  else if(handlertype == "penalty")
    {
      // Attempt to load stiffness attribute
      double k=100;
      rapidxml::xml_attribute<> *knd = nd->first_attribute("k");
      if(knd != NULL)
	{
	  if(!stringutils::extractFromString(std::string(knd->value()), k))
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'k' attribute for collision handler. Value must be numeric. Exiting." << std::endl;
	      exit(1);
	    }
	}

      double thickness=0.01;
      rapidxml::xml_attribute<> *thicknd = nd->first_attribute("thickness");
      if(thicknd != NULL)
	{
	  if(!stringutils::extractFromString(std::string(thicknd->value()), thickness))
	    {
	      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'thickness' attribute for collision handler. Value must be numeric. Exiting." << std::endl;
	      exit(1);
	    }
	}
      *handler = NULL;
      scene.insertForce(new PenaltyForce(scene, detector, k, thickness));
    }
  else
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Invalid collision handler 'type' attribute specified. Exiting." << std::endl;
    exit(1);
  }

}

void SVGSceneParser::loadIntegrator( rapidxml::xml_node<>* node, SceneStepper** scenestepper, scalar& dt )
{
  assert( node != NULL );
  
  dt = -1.0;
  
  // Attempt to locate the integrator node
  rapidxml::xml_node<>* nd = node->first_node("integrator");
  if( nd == NULL ) 
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No integrator specified. Exiting." << std::endl;
    exit(1);
  }
  
  // Attempt to load the integrator type
  rapidxml::xml_attribute<>* typend = nd->first_attribute("type"); 
  if( typend == NULL ) 
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No integrator 'type' attribute specified. Exiting." << std::endl;
    exit(1);
  }
  std::string integratortype(typend->value());
  
  if( integratortype == "explicit-euler" ) *scenestepper = new ExplicitEuler;
  else if( integratortype == "forward-backward-euler" ) *scenestepper = new SemiImplicitEuler;
  else if( integratortype == "implicit-euler" ) *scenestepper = new ImplicitEuler;
  else if( integratortype == "linearized-implicit-euler" ) *scenestepper = new LinearizedImplicitEuler;
  else
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Invalid integrator 'type' attribute specified. Exiting." << std::endl;
    exit(1);
  }

  // Attempt to load the timestep
  rapidxml::xml_attribute<>* dtnd = nd->first_attribute("dt"); 
  if( dtnd == NULL ) 
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No integrator 'dt' attribute specified. Exiting." << std::endl;
    exit(1);
  }

  dt = std::numeric_limits<scalar>::signaling_NaN();
  if( !stringutils::extractFromString(std::string(dtnd->value()),dt) )
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'dt' attribute for integrator. Value must be numeric. Exiting." << std::endl;
    exit(1);
  }

  //std::cout << "Integrator: " << (*scenestepper)->getName() << "   dt: " << dt << std::endl;
}

void SVGSceneParser::loadMaxTime( rapidxml::xml_node<>* node, scalar& max_t )
{
  assert( node != NULL );

  // Attempt to locate the duraiton node
  rapidxml::xml_node<>* nd = node->first_node("duration");
  if( nd == NULL ) 
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No duration specified. Exiting." << std::endl;
    exit(1);
  }
  
  // Attempt to load the duration value
  rapidxml::xml_attribute<>* timend = nd->first_attribute("time"); 
  if( timend == NULL ) 
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No duration 'time' attribute specified. Exiting." << std::endl;
    exit(1);
  }
  
  max_t = std::numeric_limits<scalar>::signaling_NaN();
  if( !stringutils::extractFromString(std::string(timend->value()),max_t) )
  {
    std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'time' attribute for duration. Value must be numeric. Exiting." << std::endl;
    exit(1);
  }
}

void SVGSceneParser::loadViewport(rapidxml::xml_node<>* node, renderingutils::Viewport &view)
{
  assert( node != NULL );

  if(node->first_node("viewport") )
    {
      rapidxml::xml_attribute<> *cx = node->first_node("viewport")->first_attribute("cx");
      if(cx == NULL)
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No viewport 'cx' attribute specified. Exiting." << std::endl;
	  exit(1);
	}
      if(!stringutils::extractFromString(std::string(cx->value()),view.cx))
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'cx' attribute for viewport. Value must be scalar. Exiting." << std::endl;
	  exit(1);
	}
      rapidxml::xml_attribute<> *cy = node->first_node("viewport")->first_attribute("cy");
      if(cy == NULL)
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No viewport 'cy' attribute specified. Exiting." << std::endl;
	  exit(1);
	}
      if(!stringutils::extractFromString(std::string(cy->value()),view.cy))
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'cy' attribute for viewport. Value must be scalar. Exiting." << std::endl;
	  exit(1);
	}
      rapidxml::xml_attribute<> *size = node->first_node("viewport")->first_attribute("size");
      if(size == NULL)
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No viewport 'size' attribute specified. Exiting." << std::endl;
	  exit(1);
	}
      if(!stringutils::extractFromString(std::string(size->value()),view.size))
	{
	  std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'size' attribute for viewport. Value must be scalar. Exiting." << std::endl;
	  exit(1);
	}
    }
}

void SVGSceneParser::loadMaxSimFrequency( rapidxml::xml_node<>* node, scalar& max_freq )
{
  assert( node != NULL );
  
  // Attempt to locate the duraiton node
  if( node->first_node("maxsimfreq") ) 
  {
    // Attempt to load the duration value
    rapidxml::xml_attribute<>* atrbnde = node->first_node("maxsimfreq")->first_attribute("max"); 
    if( atrbnde == NULL ) 
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m No maxsimfreq 'max' attribute specified. Exiting." << std::endl;
      exit(1);
    }

    if( !stringutils::extractFromString(std::string(atrbnde->value()),max_freq) )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse 'max' attribute for maxsimfreq. Value must be scalar. Exiting." << std::endl;
      exit(1);
    }
  }
}




void SVGSceneParser::loadBackgroundColor( rapidxml::xml_node<>* node, renderingutils::Color& color )
{
  if( rapidxml::xml_node<>* nd = node->first_node("backgroundcolor") )
  {
    // Read in the red color channel 
    double red = -1.0;
    if( nd->first_attribute("r") )
    {
      std::string attribute(nd->first_attribute("r")->value());
      if( !stringutils::extractFromString(attribute,red) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for backgroundcolor. Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for backgroundcolor. Exiting." << std::endl;
      exit(1);
    }
    
    if( red < 0.0 || red > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for backgroundcolor. Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the green color channel 
    double green = -1.0;
    if( nd->first_attribute("g") )
    {
      std::string attribute(nd->first_attribute("g")->value());
      if( !stringutils::extractFromString(attribute,green) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for backgroundcolor. Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for backgroundcolor. Exiting." << std::endl;
      exit(1);
    }
    
    if( green < 0.0 || green > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for backgroundcolor. Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the blue color channel 
    double blue = -1.0;
    if( nd->first_attribute("b") )
    {
      std::string attribute(nd->first_attribute("b")->value());
      if( !stringutils::extractFromString(attribute,blue) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for backgroundcolor. Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for backgroundcolor. Exiting." << std::endl;
      exit(1);
    }
    
    if( blue < 0.0 || blue > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for backgroundcolor. Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }

    //std::cout << red << "   " << green << "   " << blue << std::endl;

    color.r = red;
    color.g = green;
    color.b = blue;  
  }
}

void SVGSceneParser::loadParticleColors( rapidxml::xml_node<>* node, std::vector<renderingutils::Color>& particle_colors )
{
  int particlecolornum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("particlecolor"); nd; nd = nd->next_sibling("particlecolor") )
  {
    // Determine which particle this color corresponds to
    int particle = -1;
    if( nd->first_attribute("i") )
    {
      std::string attribute(nd->first_attribute("i")->value());
      if( !stringutils::extractFromString(attribute,particle) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for particlecolor " << particlecolornum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for particlecolor " << particlecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( particle < 0 || particle >= (int) particle_colors.size() )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for particlecolor " << particlecolornum << ". Invalid particle specified. Valid range is " << 0 << "..." << particle_colors.size()-1 << std::endl;
      exit(1);
    }

      
    // Read in the red color channel 
    double red = -1.0;
    if( nd->first_attribute("r") )
    {
      std::string attribute(nd->first_attribute("r")->value());
      if( !stringutils::extractFromString(attribute,red) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for particlecolor " << particlecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for particlecolor " << particlecolornum << ". Exiting." << std::endl;
      exit(1);
    }

    if( red < 0.0 || red > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for particlecolor " << particlecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the green color channel 
    double green = -1.0;
    if( nd->first_attribute("g") )
    {
      std::string attribute(nd->first_attribute("g")->value());
      if( !stringutils::extractFromString(attribute,green) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for particlecolor " << particlecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for particlecolor " << particlecolornum << ". Exiting." << std::endl;
      exit(1);
    }

    if( green < 0.0 || green > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for particlecolor " << particlecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the blue color channel 
    double blue = -1.0;
    if( nd->first_attribute("b") )
    {
      std::string attribute(nd->first_attribute("b")->value());
      if( !stringutils::extractFromString(attribute,blue) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for particlecolor " << particlecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for particlecolor " << particlecolornum << ". Exiting." << std::endl;
      exit(1);
    }

    if( blue < 0.0 || blue > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for particlecolor " << particlecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    particle_colors[particle] = renderingutils::Color(red,green,blue);
    
    ++particlecolornum;
  }
}

void SVGSceneParser::loadEdgeColors( rapidxml::xml_node<>* node, std::vector<renderingutils::Color>& edge_colors )
{
  int edgecolornum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("edgecolor"); nd; nd = nd->next_sibling("edgecolor") )
  {
    // Determine which particle this color corresponds to
    int edge = -1;
    if( nd->first_attribute("i") )
    {
      std::string attribute(nd->first_attribute("i")->value());
      if( !stringutils::extractFromString(attribute,edge) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for edgecolor " << edgecolornum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for edgecolor " << edgecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( edge < 0 || edge > (int) edge_colors.size() )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for edgecolor " << edgecolornum << ". Invalid edge specified. Valid range is " << 0 << "..." << edge_colors.size()-1 << std::endl;
      exit(1);
    }
    
    
    // Read in the red color channel 
    double red = -1.0;
    if( nd->first_attribute("r") )
    {
      std::string attribute(nd->first_attribute("r")->value());
      if( !stringutils::extractFromString(attribute,red) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for edgecolor " << edgecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for edgecolor " << edgecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( red < 0.0 || red > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for edgecolor " << edgecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the green color channel 
    double green = -1.0;
    if( nd->first_attribute("g") )
    {
      std::string attribute(nd->first_attribute("g")->value());
      if( !stringutils::extractFromString(attribute,green) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for edgecolor " << edgecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for edgecolor " << edgecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( green < 0.0 || green > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for edgecolor " << edgecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the blue color channel 
    double blue = -1.0;
    if( nd->first_attribute("b") )
    {
      std::string attribute(nd->first_attribute("b")->value());
      if( !stringutils::extractFromString(attribute,blue) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for edgecolor " << edgecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for edgecolor " << edgecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( blue < 0.0 || blue > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for edgecolor " << edgecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    //std::cout << "edge: " << edge << " r: " << red << " g: " << green << " b: " << blue << std::endl;
    
    edge_colors[edge] = renderingutils::Color(red,green,blue);
    
    ++edgecolornum;
  }  
}

void SVGSceneParser::loadHalfplaneColors( rapidxml::xml_node<>* node, std::vector<renderingutils::Color>& halfplane_colors )
{
  int halfplanecolornum = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("halfplanecolor"); nd; nd = nd->next_sibling("halfplanecolor") )
  {
    // Determine which particle this color corresponds to
    int halfplane = -1;
    if( nd->first_attribute("i") )
    {
      std::string attribute(nd->first_attribute("i")->value());
      if( !stringutils::extractFromString(attribute,halfplane) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for halfplanecolor " << halfplanecolornum << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for halfplanecolor " << halfplanecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( halfplane < 0 || halfplane > (int) halfplane_colors.size() )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for halfplanecolor " << halfplanecolornum << ". Invalid half-plane specified. Valid range is " << 0 << "..." << halfplane_colors.size()-1 << std::endl;
      exit(1);
    }
    
    
    // Read in the red color channel 
    double red = -1.0;
    if( nd->first_attribute("r") )
    {
      std::string attribute(nd->first_attribute("r")->value());
      if( !stringutils::extractFromString(attribute,red) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for halfplanecolor " << halfplanecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for halfplanecolor " << halfplanecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( red < 0.0 || red > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for halfplanecolor " << halfplanecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the green color channel 
    double green = -1.0;
    if( nd->first_attribute("g") )
    {
      std::string attribute(nd->first_attribute("g")->value());
      if( !stringutils::extractFromString(attribute,green) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for halfplanecolor " << halfplanecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for halfplanecolor " << halfplanecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( green < 0.0 || green > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for halfplanecolor " << halfplanecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the blue color channel 
    double blue = -1.0;
    if( nd->first_attribute("b") )
    {
      std::string attribute(nd->first_attribute("b")->value());
      if( !stringutils::extractFromString(attribute,blue) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for halfplanecolor " << halfplanecolornum << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for halfplanecolor " << halfplanecolornum << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( blue < 0.0 || blue > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for halfplanecolor " << halfplanecolornum << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    halfplane_colors[halfplane] = renderingutils::Color(red,green,blue);
    
    ++halfplanecolornum;
  }  
}

void SVGSceneParser::loadParticlePaths( rapidxml::xml_node<>* node, double dt, std::vector<renderingutils::ParticlePath>& particle_paths )
{
  int numpaths = 0;
  for( rapidxml::xml_node<>* nd = node->first_node("particlepath"); nd; nd = nd->next_sibling("particlepath") )
  {
    // Determine which particle this color corresponds to
    int particle = -1;
    if( nd->first_attribute("i") )
    {
      std::string attribute(nd->first_attribute("i")->value());
      if( !stringutils::extractFromString(attribute,particle) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for particlepath " << numpaths << ". Value must be integer. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of i attribute for particlepath " << numpaths << ". Exiting." << std::endl;
      exit(1);
    }
    
    // How long the path should be buffered for
    double duration = -1.0;
    if( nd->first_attribute("duration") )
    {
      std::string attribute(nd->first_attribute("duration")->value());
      if( !stringutils::extractFromString(attribute,duration) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of duration attribute for particlepath " << numpaths << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of duration attribute for particlepath " << numpaths << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( duration < 0.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of duration attribute for particlepath " << numpaths << ". Value must be positive scalar." << std::endl;
      exit(1);
    }
    
    
    // Read in the red color channel 
    double red = -1.0;
    if( nd->first_attribute("r") )
    {
      std::string attribute(nd->first_attribute("r")->value());
      if( !stringutils::extractFromString(attribute,red) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for particlepath " << numpaths << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for particlepath " << numpaths << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( red < 0.0 || red > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of r attribute for particlepath " << numpaths << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the green color channel 
    double green = -1.0;
    if( nd->first_attribute("g") )
    {
      std::string attribute(nd->first_attribute("g")->value());
      if( !stringutils::extractFromString(attribute,green) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for particlepath " << numpaths << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for particlepath " << numpaths << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( green < 0.0 || green > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of g attribute for particlepath " << numpaths << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    
    // Read in the blue color channel 
    double blue = -1.0;
    if( nd->first_attribute("b") )
    {
      std::string attribute(nd->first_attribute("b")->value());
      if( !stringutils::extractFromString(attribute,blue) )
      {
        std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for particlepath " << numpaths << ". Value must be scalar. Exiting." << std::endl;
        exit(1);
      }        
    }
    else
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for particlepath " << numpaths << ". Exiting." << std::endl;
      exit(1);
    }
    
    if( blue < 0.0 || blue > 1.0 )
    {
      std::cerr << "\033[31;1mERROR IN XMLSCENEPARSER:\033[m Failed to parse value of b attribute for particlepath " << numpaths << ". Invalid color specified. Valid range is " << 0.0 << "..." << 1.0 << std::endl;
      exit(1);
    }
    
    particle_paths.push_back(renderingutils::ParticlePath( particle, ceil(duration/dt), renderingutils::Color(red,green,blue) ));

    //std::cout << particle << " " << duration << " " << red << " " << green << " " << blue << std::endl;
    
    ++numpaths;
  }  
}



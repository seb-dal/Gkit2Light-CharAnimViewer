#pragma once
#include <vector>
#include <src/master_CharAnim/BVH.h>
#include <map>

class MotionGraph
{
protected:
	//! L'ensemble des BVH du graphe d'animation
	std::vector<chara::BVH> m_BVH;

	//! Un noeud du graphe d'animation est repéré par un entier=un identifiant
	typedef int GrapheNodeID;

	//! Une animation BVH est repérée par un identifiant=un entier 
	typedef int BVH_ID;

	//! Un noeud du graphe contient l'identifiant de l'animation, le numéro 
	//! de la frame et les identifiants des noeuds successeurs 
	//! Remarque : du code plus "joli" aurait créer une classe CAGrapheNode
	struct GrapheNode
	{
		BVH_ID id_bvh;
		int frame;
		std::vector<GrapheNodeID> ids_next;     //! Liste des nœuds successeurs 
	};


	//! Tous les noeuds du graphe d'animation
	std::vector<GrapheNode> m_GrapheNode;

	std::map<chara::BVH, BVH_ID> l_BVH_ID;

public:
	BVH_ID addMotion(chara::BVH& motion);
	BVH_ID getMotion_ID(chara::BVH& motion);

	int getFrame(chara::BVH&)

};


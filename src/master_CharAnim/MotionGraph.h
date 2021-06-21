#pragma once
#include <vector>
#include <src/master_CharAnim/BVH.h>
#include <map>


class Skeleton;
class MotionGraph
{
protected:
	//! L'ensemble des BVH du graphe d'animation
	std::vector<chara::BVH> m_BVH;

	//! Un noeud du graphe d'animation est repéré par un entier=un identifiant
	typedef int GrapheNodeID;

	//! Une animation BVH est repérée par un identifiant=un entier 
	typedef int BVH_ID;

public:
	//! Un noeud du graphe contient l'identifiant de l'animation, le numéro 
	//! de la frame et les identifiants des noeuds successeurs 
	//! Remarque : du code plus "joli" aurait créer une classe CAGrapheNode
	struct GrapheNode {
		BVH_ID id_bvh;
		int frame;
		std::vector<GrapheNodeID> ids_next;     //! Liste des nœuds successeurs 
	};

protected:

	//! Tous les noeuds du graphe d'animation
	std::vector<GrapheNode> m_GrapheNode;
	std::map<std::string, GrapheNode> m_GrapheNodeGet;
public:
	BVH_ID addMotion(chara::BVH& motion);

	GrapheNode& getNextFrame(GrapheNode& actu, BVH_ID objectif);

	static std::string tokey(BVH_ID id, int frame) {
		std::string key;
		key.append(std::to_string(id)).append(" ").append(std::to_string(frame));
		return key;
	}

	GrapheNode& getNode(BVH_ID id, int frame) {
		std::string key = tokey(id, frame);
		if (m_GrapheNodeGet.find(key) != m_GrapheNodeGet.end()) {
			return m_GrapheNodeGet[key];
		}
		return m_GrapheNode.at(0);
	}

};


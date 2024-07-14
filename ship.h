//
// Created by Waseem on 06/07/2024.
//

#ifndef WET1_SHIP_H
#define WET1_SHIP_H
#include "pirates.h"
#include "AVLTree.h"
#include <memory>
using namespace std;

class Ship{
private:
    int m_shipId;
    int m_cannons;
    int m_nextInOrder;
    int m_coinDiff;
    int m_richest;

public:
    AVLTree<int,shared_ptr<Pirates>> pirates_id_tree;
    AVLTree<int,shared_ptr<Pirates>> pirates_order_tree;
    AVLTree<int, AVLTree<int,shared_ptr<Pirates>>> pirates_treasure_tree;

    // update constructor and destructor
    Ship(int sID, int can)
            : m_shipId(sID), m_cannons(can), m_nextInOrder(0), m_coinDiff(0) {

    }

    int getShipID() const { return m_shipId; }
    int getCannons() const { return m_cannons; }
    int getNextInOrder() const { return m_nextInOrder; }
    int getCoinDiff() const { return m_coinDiff; }
    int getRichestID() const{return m_richest; }
    int get_size() const{
        return pirates_id_tree.getSize();
    }

    void setShipID(int sID) { m_shipId = sID; }
    void setCannons(int can) { m_cannons = can; }
    void setNextInOrder(int next) { m_nextInOrder = next; }
    void setCoinDiff(int coinDiff) { m_coinDiff = coinDiff; }
    void setRichestID(int richest){m_richest = richest; }

    void removePirate(int id,int treasure,int order){
        pirates_id_tree.remove(id);
        pirates_order_tree.remove(order);
        node<int, AVLTree<int, shared_ptr<Pirates>>> *currentPirateTreasureTree = pirates_treasure_tree.find(treasure);
        currentPirateTreasureTree->m_data.remove(id);
        if(currentPirateTreasureTree->m_data.getSize() == 0)
            pirates_treasure_tree.remove(treasure);
    }

};
#endif //WET1_SHIP_H

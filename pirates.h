//
// Created by Waseem on 06/07/2024.
//

#ifndef WET1_PIRATES_H
#define WET1_PIRATES_H
#include <memory>
using namespace std;
#include "ship.h"
class Ship;

class Pirates {
private:
    int m_piratesID;
    int m_shipID;
    int m_treasure;
    int m_order;
    shared_ptr<Ship> m_source_ship;
public:
    Pirates(int pID, int sID, int tr,int ord,shared_ptr<Ship> ship)
            : m_piratesID(pID), m_shipID(sID), m_treasure(tr), m_order(ord),m_source_ship(ship) {}
    ~Pirates() = default;

    int getPirateID() const { return m_piratesID; }
    int getShipID() const { return m_shipID; }
    int getTreasure() const { return m_treasure; }
    int getOrder() const { return m_order; }
    shared_ptr<Ship> getSourceShip() const {return m_source_ship;}

    void setPirateID(int pID) { m_piratesID = pID; }
    void setShipID(int sID) { m_shipID = sID; }
    void setTreasure(int tr) { m_treasure = tr; }
    void setOrder(int ord) { m_order = ord; }
    void setSourceShip(shared_ptr<Ship> ship){
        m_source_ship = ship;}
};

#endif //WET1_PIRATES_H

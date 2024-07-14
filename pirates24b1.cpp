#include "pirates24b1.h"



Ocean::Ocean() = default;

Ocean::~Ocean() = default;

StatusType Ocean::add_ship(int shipId, int cannons)
{
    if (shipId <= 0 || cannons < 0)
    {
        return StatusType::INVALID_INPUT;
    }
   try
    {
        Ship ship(shipId, cannons);
        bool success = m_ships.insert(shipId, make_shared<Ship>(ship));
        if (!success) {
            return StatusType::FAILURE;
        }
        return StatusType::SUCCESS;
    }
    catch(...)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::remove_ship(int shipId)
{
    if(shipId <= 0)
        return StatusType::INVALID_INPUT;
    try {
        node<int, shared_ptr<Ship>> *current = m_ships.find(shipId);
        if (current == nullptr) {
            return StatusType::FAILURE;
        }
        if (current->m_data->pirates_id_tree.getSize() != 0) {
            return StatusType::FAILURE;
        }
        m_ships.remove(shipId);
        return StatusType::SUCCESS;
    }
    catch(...)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::add_pirate(int pirateId, int shipId, int treasure)
{
    if(pirateId <= 0 || shipId <= 0)
        return StatusType::INVALID_INPUT;
    try {
        node<int, shared_ptr<Ship>> *currentShip = m_ships.find(shipId);
        if (currentShip == nullptr)
            return StatusType::FAILURE;
        node<int, shared_ptr<Pirates>> *currentPirate = m_pirates.find(pirateId);
        if (currentPirate != nullptr) {
            return StatusType::FAILURE;
        }
        treasure = treasure - (currentShip->m_data->getCoinDiff());
        Pirates pirate(pirateId, shipId, treasure, currentShip->m_data->getNextInOrder(), currentShip->m_data);
        m_pirates.insert(pirateId, make_shared<Pirates>(pirate));

        currentShip->m_data->pirates_id_tree.insert(pirateId, make_shared<Pirates>(pirate));
        currentShip->m_data->pirates_order_tree.insert(currentShip->m_data->getNextInOrder(), make_shared<Pirates>(pirate));
        currentShip->m_data->setNextInOrder(currentShip->m_data->getNextInOrder() + 1);

        node<int, AVLTree<int, shared_ptr<Pirates>>> *currentPirateTreasureTree = currentShip->m_data->pirates_treasure_tree.find(
                treasure);
        if (currentPirateTreasureTree == nullptr) {
            AVLTree<int, shared_ptr<Pirates>> tree;
            tree.insert(pirateId, make_shared<Pirates>(pirate));
            currentShip->m_data->pirates_treasure_tree.insert(treasure, tree);
        } else {
            currentPirateTreasureTree->m_data.insert(pirateId, make_shared<Pirates>(pirate));
        }
        currentShip->m_data->setRichestID(currentShip->m_data->pirates_treasure_tree.searchMax()->m_data.searchMax()->m_key);
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType Ocean::remove_pirate(int pirateId)
{
    if(pirateId <= 0 ){
        return StatusType::INVALID_INPUT;
    }
    try {
        node<int, shared_ptr<Pirates>> *current = m_pirates.find(pirateId);
        if (current == nullptr) {
            return StatusType::FAILURE;
        }
        current->m_data->getSourceShip()->removePirate(pirateId,
                                                       current->m_data->getTreasure(),current->m_data->getOrder());
        m_pirates.remove(pirateId);
        if(current->m_data->getSourceShip()->getRichestID() == pirateId) {
            current->m_data->getSourceShip()->setRichestID(current->m_data->getSourceShip()
                                                                   ->pirates_treasure_tree.searchMax()->m_data.searchMax()->m_key);
        }
    }
    catch(...)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType Ocean::treason(int sourceShipId, int destShipId)
{
    if((sourceShipId <= 0) || (destShipId <= 0) || (destShipId == sourceShipId))
        return StatusType::INVALID_INPUT;
    try {
        node<int, shared_ptr<Ship>> *source = m_ships.find(sourceShipId);
        node<int, shared_ptr<Ship>> *target = m_ships.find(destShipId);
        if (source == nullptr || target == nullptr)

            return StatusType::FAILURE;
        if (source->m_data->pirates_id_tree.getSize() == 0)
            return StatusType::FAILURE;

        node<int, shared_ptr<Pirates>> *s = source->m_data->pirates_order_tree.searchMin();
        int Id = s->m_key;
        int ttreasure = s->m_data->getTreasure();
        this->remove_pirate(s->m_key);
        this->add_pirate(Id, destShipId, ttreasure);
        source->m_data->setRichestID(source->m_data->pirates_treasure_tree.searchMax()->m_data.searchMax()->m_key);
        target->m_data->setRichestID(target->m_data->pirates_treasure_tree.searchMax()->m_data.searchMax()->m_key);
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType Ocean::update_pirate_treasure(int pirateId, int change)
{
    if(pirateId <= 0 ){
        return StatusType::INVALID_INPUT;
    }
    node<int,shared_ptr<Pirates>>* current = m_pirates.find(pirateId);
    if(current == nullptr){
        return StatusType::FAILURE;
    }
    shared_ptr<Ship> currentShip =  current->m_data->getSourceShip();
    int current_treasure = current->m_data->getTreasure();
    node<int,AVLTree<int,shared_ptr<Pirates>>>* treasure_Tree = currentShip->pirates_treasure_tree.find(current_treasure);

    treasure_Tree->m_data.remove(pirateId);
    if(treasure_Tree->m_data.getSize() == 0)
        treasure_Tree->m_data.remove(change);
    change += current_treasure - current->m_data->getSourceShip()->getCoinDiff();
    current->m_data->setTreasure(change);

    try {
        node<int, AVLTree<int, shared_ptr<Pirates>>> *currentPirateTreasureTree = currentShip->pirates_treasure_tree.find(
                change);
        if (currentPirateTreasureTree == nullptr) {
            AVLTree<int, shared_ptr<Pirates>> tree;
            tree.insert(pirateId, current->m_data);
            currentShip->pirates_treasure_tree.insert(change, tree);
        } else {
            currentPirateTreasureTree->m_data.insert(pirateId,current->m_data);
        }
        currentShip->setRichestID(currentShip->pirates_treasure_tree.searchMax()->m_data.searchMax()->m_key);
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> Ocean::get_treasure(int pirateId)
{
    if(pirateId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try{
        if (m_pirates.find(pirateId) == nullptr)
            return output_t<int>(StatusType::FAILURE);
    }
    catch(...){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    return  output_t<int>(m_pirates.find(pirateId)->m_data->getTreasure() +
    m_pirates.find(pirateId)->m_data->getSourceShip()->getCoinDiff());

}

output_t<int> Ocean::get_cannons(int shipId)
{
    if(shipId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try{
        if (m_ships.find(shipId) == nullptr)
            return output_t<int>(StatusType::FAILURE);
    }
    catch(...) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    return output_t<int>(m_ships.find(shipId)->m_data->getCannons());
}

output_t<int> Ocean::get_richest_pirate(int shipId)
{
    if(shipId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try{
        if (m_ships.find(shipId) == nullptr)
            return output_t<int>(StatusType::FAILURE);
        if(m_ships.find(shipId)->m_data->pirates_id_tree.getSize() == 0)
            return output_t<int>(StatusType::FAILURE);
    }
    catch(...) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    return output_t<int>(m_ships.find(shipId)->m_data->getRichestID());
}

StatusType Ocean::ships_battle(int shipId1,int shipId2)
{
    if(shipId1 <= 0 || shipId2 <= 0 || shipId1 == shipId2){
        return StatusType::INVALID_INPUT;
    }
    node<int,shared_ptr<Ship>>* ship1 = m_ships.find(shipId1);
    node<int,shared_ptr<Ship>>* ship2 = m_ships.find(shipId2);
    if(ship1 == nullptr || ship2 == nullptr){
        return StatusType::FAILURE;
    }
    int crew1 = ship1->m_data->get_size();
    int crew2 = ship2->m_data->get_size();
    int coinDeff1 = ship1->m_data->getCoinDiff();
    int coinDeff2 = ship2->m_data->getCoinDiff();
    int power1 = min(ship1->m_data->getCannons(),crew1);
    int power2 = min(ship2->m_data->getCannons(),crew2);
    if(power1 < power2){
        ship1->m_data->setCoinDiff(coinDeff1 - crew2);
        ship2->m_data->setCoinDiff(coinDeff2 + crew1);
    }else if (power1 > power2){
        ship1->m_data->setCoinDiff(coinDeff1 + crew2);
        ship2->m_data->setCoinDiff(coinDeff2 - crew1);
    }
    return StatusType::SUCCESS;
}




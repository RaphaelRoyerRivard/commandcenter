#include "ProductionManager.h"
#include "Util.h"
#include "CCBot.h"

ProductionManager::ProductionManager(CCBot & bot)
    : m_bot             (bot)
    , m_queue           (bot)
	, m_initialBuildOrderFinished(false)
{

}

void ProductionManager::setBuildOrder(const BuildOrder & buildOrder)
{
    m_queue.clearAll();

    for (size_t i(0); i<buildOrder.size(); ++i)
    {
        m_queue.queueAsLowestPriority(buildOrder[i], true);
    }
}


void ProductionManager::onStart()
{
    setBuildOrder(m_bot.Strategy().getOpeningBookBuildOrder());
	if (m_queue.isEmpty())
		Util::DisplayError("Initial build order is empty.", "0x00000003", m_bot, true);
	supplyProvider = Util::GetSupplyProvider(m_bot.GetSelfRace(), m_bot);
	supplyProviderType = MetaType(supplyProvider, m_bot);

	workerType = Util::GetWorkerType(m_bot.GetSelfRace(), m_bot);
	workerMetatype = MetaType(workerType, m_bot);
	
	switch (m_bot.GetSelfRace())
	{
		case CCRace::Terran:
		{
			possibleUpgrades = {
				{ MetaTypeEnum::TerranInfantryWeaponsLevel1, MetaTypeEnum::TerranInfantryWeaponsLevel2, MetaTypeEnum::TerranInfantryWeaponsLevel3 },
				{ MetaTypeEnum::TerranInfantryArmorsLevel1, MetaTypeEnum::TerranInfantryArmorsLevel2, MetaTypeEnum::TerranInfantryArmorsLevel3 },
				{ MetaTypeEnum::TerranVehicleWeaponsLevel1, MetaTypeEnum::TerranVehicleWeaponsLevel2, MetaTypeEnum::TerranVehicleWeaponsLevel3 },
				{ MetaTypeEnum::TerranShipWeaponsLevel1, MetaTypeEnum::TerranShipWeaponsLevel2, MetaTypeEnum::TerranShipWeaponsLevel3 },
				{ MetaTypeEnum::TerranVehicleAndShipArmorsLevel1, MetaTypeEnum::TerranVehicleAndShipArmorsLevel2, MetaTypeEnum::TerranVehicleAndShipArmorsLevel3 },
			};

			reversePossibleUpgrades = {
				{ MetaTypeEnum::TerranInfantryWeaponsLevel3, MetaTypeEnum::TerranInfantryWeaponsLevel2, MetaTypeEnum::TerranInfantryWeaponsLevel1 },
				{ MetaTypeEnum::TerranInfantryArmorsLevel3, MetaTypeEnum::TerranInfantryArmorsLevel2, MetaTypeEnum::TerranInfantryArmorsLevel1 },
				{ MetaTypeEnum::TerranVehicleWeaponsLevel3, MetaTypeEnum::TerranVehicleWeaponsLevel2, MetaTypeEnum::TerranVehicleWeaponsLevel1 },
				{ MetaTypeEnum::TerranShipWeaponsLevel3, MetaTypeEnum::TerranShipWeaponsLevel2, MetaTypeEnum::TerranShipWeaponsLevel1 },
				{ MetaTypeEnum::TerranVehicleAndShipArmorsLevel3, MetaTypeEnum::TerranVehicleAndShipArmorsLevel2, MetaTypeEnum::TerranVehicleAndShipArmorsLevel1 },
			};

			alternateUpgrades[MetaTypeEnum::TerranInfantryWeaponsLevel1.getName()] = MetaTypeEnum::TerranInfantryArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::TerranInfantryArmorsLevel1.getName()] = MetaTypeEnum::TerranInfantryWeaponsLevel1;
			alternateUpgrades[MetaTypeEnum::TerranInfantryWeaponsLevel2.getName()] = MetaTypeEnum::TerranInfantryArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::TerranInfantryArmorsLevel2.getName()] = MetaTypeEnum::TerranInfantryWeaponsLevel2;
			alternateUpgrades[MetaTypeEnum::TerranInfantryWeaponsLevel3.getName()] = MetaTypeEnum::TerranInfantryArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::TerranInfantryArmorsLevel3.getName()] = MetaTypeEnum::TerranInfantryWeaponsLevel3;

			alternateUpgrades[MetaTypeEnum::TerranShipWeaponsLevel1.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::TerranVehicleWeaponsLevel1.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::TerranShipWeaponsLevel2.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::TerranVehicleWeaponsLevel2.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::TerranShipWeaponsLevel3.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::TerranVehicleWeaponsLevel3.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel3;

			alternateUpgrades[MetaTypeEnum::TerranVehicleAndShipArmorsLevel1.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel2;//Doesn't have an alternate
			alternateUpgrades[MetaTypeEnum::TerranVehicleAndShipArmorsLevel2.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::TerranVehicleAndShipArmorsLevel3.getName()] = MetaTypeEnum::TerranVehicleAndShipArmorsLevel3;
			break;
		}
		case CCRace::Protoss:
		{
			possibleUpgrades = {
				{ MetaTypeEnum::ProtossGroundWeaponsLevel1, MetaTypeEnum::ProtossGroundWeaponsLevel2, MetaTypeEnum::ProtossGroundWeaponsLevel3 },
				{ MetaTypeEnum::ProtossGroundArmorsLevel1, MetaTypeEnum::ProtossGroundArmorsLevel2, MetaTypeEnum::ProtossGroundArmorsLevel3 },
				{ MetaTypeEnum::ProtossAirWeaponsLevel1, MetaTypeEnum::ProtossAirWeaponsLevel2, MetaTypeEnum::ProtossAirWeaponsLevel3 },
				{ MetaTypeEnum::ProtossAirArmorsLevel1, MetaTypeEnum::ProtossAirArmorsLevel2, MetaTypeEnum::ProtossAirArmorsLevel3 },
				{ MetaTypeEnum::ProtossShieldsLevel1, MetaTypeEnum::ProtossShieldsLevel2, MetaTypeEnum::ProtossShieldsLevel3 },
			};

			reversePossibleUpgrades = {
				{ MetaTypeEnum::ProtossGroundWeaponsLevel3, MetaTypeEnum::ProtossGroundWeaponsLevel2, MetaTypeEnum::ProtossGroundWeaponsLevel1 },
				{ MetaTypeEnum::ProtossGroundArmorsLevel3, MetaTypeEnum::ProtossGroundArmorsLevel2, MetaTypeEnum::ProtossGroundArmorsLevel1 },
				{ MetaTypeEnum::ProtossAirWeaponsLevel3, MetaTypeEnum::ProtossAirWeaponsLevel2, MetaTypeEnum::ProtossAirWeaponsLevel1 },
				{ MetaTypeEnum::ProtossAirArmorsLevel3, MetaTypeEnum::ProtossAirArmorsLevel2, MetaTypeEnum::ProtossAirArmorsLevel1 },
				{ MetaTypeEnum::ProtossShieldsLevel3, MetaTypeEnum::ProtossShieldsLevel2, MetaTypeEnum::ProtossShieldsLevel1 },
			};

			alternateUpgrades[MetaTypeEnum::ProtossGroundWeaponsLevel1.getName()] = MetaTypeEnum::ProtossGroundArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::ProtossGroundArmorsLevel1.getName()] = MetaTypeEnum::ProtossGroundWeaponsLevel1;
			alternateUpgrades[MetaTypeEnum::ProtossGroundWeaponsLevel2.getName()] = MetaTypeEnum::ProtossGroundArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::ProtossGroundArmorsLevel2.getName()] = MetaTypeEnum::ProtossGroundWeaponsLevel2;
			alternateUpgrades[MetaTypeEnum::ProtossGroundWeaponsLevel3.getName()] = MetaTypeEnum::ProtossGroundArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::ProtossGroundArmorsLevel3.getName()] = MetaTypeEnum::ProtossGroundWeaponsLevel3;

			alternateUpgrades[MetaTypeEnum::ProtossAirWeaponsLevel1.getName()] = MetaTypeEnum::ProtossAirArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::ProtossAirArmorsLevel1.getName()] = MetaTypeEnum::ProtossAirWeaponsLevel1;
			alternateUpgrades[MetaTypeEnum::ProtossAirWeaponsLevel2.getName()] = MetaTypeEnum::ProtossAirArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::ProtossAirArmorsLevel2.getName()] = MetaTypeEnum::ProtossAirWeaponsLevel2;
			alternateUpgrades[MetaTypeEnum::ProtossAirWeaponsLevel3.getName()] = MetaTypeEnum::ProtossAirArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::ProtossAirArmorsLevel3.getName()] = MetaTypeEnum::ProtossAirWeaponsLevel3;

			alternateUpgrades[MetaTypeEnum::ProtossShieldsLevel1.getName()] = MetaTypeEnum::ProtossShieldsLevel2;//Doesn't have an alternate
			alternateUpgrades[MetaTypeEnum::ProtossShieldsLevel2.getName()] = MetaTypeEnum::ProtossShieldsLevel3;
			alternateUpgrades[MetaTypeEnum::ProtossShieldsLevel3.getName()] = MetaTypeEnum::ProtossShieldsLevel3;
			break;
		}
		case CCRace::Zerg:
		{
			possibleUpgrades = {
				{ MetaTypeEnum::ZergMeleeWeaponsLevel1, MetaTypeEnum::ZergMeleeWeaponsLevel2, MetaTypeEnum::ZergMeleeWeaponsLevel3 },
				{ MetaTypeEnum::ZergMissileWeaponsLevel1, MetaTypeEnum::ZergMissileWeaponsLevel2, MetaTypeEnum::ZergMissileWeaponsLevel3 },
				{ MetaTypeEnum::ZergGroundArmorsLevel1, MetaTypeEnum::ZergGroundArmorsLevel2, MetaTypeEnum::ZergGroundArmorsLevel3 },
				{ MetaTypeEnum::ZergFlyerWeaponsLevel1, MetaTypeEnum::ZergFlyerWeaponsLevel2, MetaTypeEnum::ZergFlyerWeaponsLevel3 },
				{ MetaTypeEnum::ZergFlyerArmorsLevel1, MetaTypeEnum::ZergFlyerArmorsLevel2, MetaTypeEnum::ZergFlyerArmorsLevel3 },
			};

			reversePossibleUpgrades = {
				{ MetaTypeEnum::ZergMeleeWeaponsLevel3, MetaTypeEnum::ZergMeleeWeaponsLevel2, MetaTypeEnum::ZergMeleeWeaponsLevel1 },
				{ MetaTypeEnum::ZergMissileWeaponsLevel3, MetaTypeEnum::ZergMissileWeaponsLevel2, MetaTypeEnum::ZergMissileWeaponsLevel1 },
				{ MetaTypeEnum::ZergGroundArmorsLevel3, MetaTypeEnum::ZergGroundArmorsLevel2, MetaTypeEnum::ZergGroundArmorsLevel1 },
				{ MetaTypeEnum::ZergFlyerWeaponsLevel3, MetaTypeEnum::ZergFlyerWeaponsLevel2, MetaTypeEnum::ZergFlyerWeaponsLevel1 },
				{ MetaTypeEnum::ZergFlyerArmorsLevel3, MetaTypeEnum::ZergFlyerArmorsLevel2, MetaTypeEnum::ZergFlyerArmorsLevel1 },
			};

			alternateUpgrades[MetaTypeEnum::ZergMeleeWeaponsLevel1.getName()] = MetaTypeEnum::ZergGroundArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel1.getName()] = MetaTypeEnum::ZergMeleeWeaponsLevel1;
			alternateUpgrades[MetaTypeEnum::ZergMeleeWeaponsLevel2.getName()] = MetaTypeEnum::ZergGroundArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel2.getName()] = MetaTypeEnum::ZergMeleeWeaponsLevel2;
			alternateUpgrades[MetaTypeEnum::ZergMeleeWeaponsLevel3.getName()] = MetaTypeEnum::ZergGroundArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel3.getName()] = MetaTypeEnum::ZergMeleeWeaponsLevel3;

			alternateUpgrades[MetaTypeEnum::ZergMissileWeaponsLevel1.getName()] = MetaTypeEnum::ZergGroundArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel1.getName()] = MetaTypeEnum::ZergMissileWeaponsLevel1;
			alternateUpgrades[MetaTypeEnum::ZergMissileWeaponsLevel2.getName()] = MetaTypeEnum::ZergGroundArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel2.getName()] = MetaTypeEnum::ZergMissileWeaponsLevel2;
			alternateUpgrades[MetaTypeEnum::ZergMissileWeaponsLevel3.getName()] = MetaTypeEnum::ZergGroundArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel3.getName()] = MetaTypeEnum::ZergMissileWeaponsLevel3;

			alternateUpgrades[MetaTypeEnum::ZergFlyerWeaponsLevel1.getName()] = MetaTypeEnum::ZergFlyerArmorsLevel1;
			alternateUpgrades[MetaTypeEnum::ZergFlyerArmorsLevel1.getName()] = MetaTypeEnum::ZergFlyerWeaponsLevel1;
			alternateUpgrades[MetaTypeEnum::ZergFlyerWeaponsLevel2.getName()] = MetaTypeEnum::ZergFlyerArmorsLevel2;
			alternateUpgrades[MetaTypeEnum::ZergFlyerArmorsLevel2.getName()] = MetaTypeEnum::ZergFlyerWeaponsLevel2;
			alternateUpgrades[MetaTypeEnum::ZergFlyerWeaponsLevel3.getName()] = MetaTypeEnum::ZergFlyerArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::ZergFlyerArmorsLevel3.getName()] = MetaTypeEnum::ZergFlyerWeaponsLevel3;

			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel1.getName()] = MetaTypeEnum::ZergGroundArmorsLevel2;//Doesn't have an alternate
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel2.getName()] = MetaTypeEnum::ZergGroundArmorsLevel3;
			alternateUpgrades[MetaTypeEnum::ZergGroundArmorsLevel3.getName()] = MetaTypeEnum::ZergGroundArmorsLevel3;
			break;
		}
	}
}

void ProductionManager::onFrame()
{
	m_bot.StartProfiling("1.0 lowPriorityChecks");
	lowPriorityChecks();
	validateUpgradesProgress();
	m_bot.StopProfiling("1.0 lowPriorityChecks");
	m_bot.StartProfiling("2.0 manageBuildOrderQueue");
    manageBuildOrderQueue();
	m_bot.StopProfiling("2.0 manageBuildOrderQueue");
	m_bot.StartProfiling("3.0 QueueDeadBuildings");
	QueueDeadBuildings();
	m_bot.StopProfiling("3.0 QueueDeadBuildings");

    // TODO: if nothing is currently building, get a new goal from the strategy manager
    // TODO: triggers for game things like cloaked units etc

    drawProductionInformation();
}

// on unit destroy
void ProductionManager::onUnitDestroy(const Unit & unit)
{
    // TODO: might have to re-do build order if a vital unit died
}

void ProductionManager::manageBuildOrderQueue()
{
    // if there is nothing in the queue, oh well
	if (!m_initialBuildOrderFinished && m_queue.isEmpty())
	{
		m_initialBuildOrderFinished = true;
	}

	if(!m_initialBuildOrderFinished && m_bot.Strategy().isWorkerRushed())
	{
		m_initialBuildOrderFinished = true;
		m_queue.clearAll();
	}

	m_bot.StartProfiling("2.1   putImportantBuildOrderItemsInQueue");
	if(m_initialBuildOrderFinished && m_bot.Config().AutoCompleteBuildOrder)
    {
		putImportantBuildOrderItemsInQueue();
    }
	m_bot.StopProfiling("2.1   putImportantBuildOrderItemsInQueue");

	if (m_queue.isEmpty())
		return;

	m_bot.StartProfiling("2.2   checkQueue");
    // the current item to be used
    BuildOrderItem currentItem = m_queue.getHighestPriorityItem();
	int highestPriority = currentItem.priority;
	int lowestMineralReq = -1;
	int lowestGasReq = -1;
	int additionalReservedMineral = 0;
	int additionalReservedGas = 0;
	bool isSupplyCap = false;

	//Dont reserve ressources for worker or less important things
	if (highestPriority <= 2)//2 == scv priority
	{
		lowestMineralReq = 0;
		lowestGasReq = 0;
	}
	
    // while there is still something left in the queue
    while (!m_queue.isEmpty())
    {
		//Get the lowest price for any top priority item in the queue.
		if (currentItem.priority == highestPriority)
		{
			if (lowestMineralReq == -1 || lowestMineralReq > m_bot.Data(currentItem.type).mineralCost)
			{
				lowestMineralReq = m_bot.Data(currentItem.type).mineralCost;
			}
			if (lowestGasReq == -1 || lowestGasReq > m_bot.Data(currentItem.type).gasCost)
			{
				lowestGasReq = m_bot.Data(currentItem.type).gasCost;
			}
		}

		//If we currently have a high priority, do not reserve ressources. Otherwise reserve ressources
		if (highestPriority == currentItem.priority)
		{
			additionalReservedMineral = 0;
			additionalReservedGas = 0;
		}
		else
		{
			additionalReservedMineral = lowestMineralReq;
			additionalReservedGas = lowestGasReq;
		}

		//check if we have the prerequirements.
		if (!hasRequired(currentItem.type, true) || !hasProducer(currentItem.type, true))
		{
			m_bot.StartProfiling("2.2.1     fixBuildOrderDeadlock");
			fixBuildOrderDeadlock(currentItem);
			currentItem = m_queue.getHighestPriorityItem();
			m_bot.StopProfiling("2.2.1     fixBuildOrderDeadlock");
			continue;
		}

		if (currentlyHasRequirement(currentItem.type))
		{
			//TODO: TEMP build barrack away from the ramp to protect it from worker rush
			/*if (!firstBarrackBuilt && currentItem.type == MetaTypeEnum::Barracks && m_bot.GetPlayerRace(Players::Enemy) == CCRace::Protoss &&
				meetsReservedResourcesWithExtra(MetaTypeEnum::Barracks, 0, 0, additionalReservedMineral, additionalReservedGas))
			{
				firstBarrackBuilt = true;

				auto baseLocation = m_bot.Bases().getPlayerStartingBaseLocation(Players::Self);
				auto & supplyDepots = m_bot.GetAllyUnits(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
				
				if (!supplyDepots.empty())//If we have a depot
				{
					auto & supplyDepot = supplyDepots[0];

					auto basePosition = baseLocation->getDepotPosition();
					auto point = supplyDepot.getTilePosition();
					CCTilePosition target = CCTilePosition(basePosition.x + (basePosition.x - point.x), basePosition.y + (basePosition.y - point.y));
					const CCPosition mapMin = m_bot.Map().mapMin();
					const CCPosition mapMax = m_bot.Map().mapMax();
					if (target.x < mapMin.x)
					{
						target.x = mapMin.x;
					}
					else if (target.x >= mapMax.x)
					{
						target.x = mapMax.x - 1;
					}
					if (target.y < mapMin.y)
					{
						target.y = mapMin.y;
					}
					else if (target.y >= mapMax.y)
					{
						target.y = mapMax.y - 1;
					}

					Unit producer = getProducer(currentItem.type);
					if (create(producer, currentItem, target))
					{
						m_queue.removeCurrentHighestPriorityItem();
					}

					break;
				}				
			}*/

			//Check if we already have an idle production building of that type
			bool idleProductionBuilding = false;
#ifndef NO_UNITS
			if(currentItem.type.isBuilding())
			{
				auto productionBuildingTypes = getProductionBuildingTypes();
				const sc2::UnitTypeID itemType = currentItem.type.getUnitType().getAPIUnitType();

				//If its a production building
				if(std::find(productionBuildingTypes.begin(), productionBuildingTypes.end(), itemType) != productionBuildingTypes.end())
				{
					auto & productionBuildings = m_bot.GetAllyUnits(itemType);
					for(auto & productionBuilding : productionBuildings)
					{
						//Check if this building is idle
						auto & orders = productionBuilding.getUnitPtr()->orders;
						if(orders.empty() || orders[0].ability_id == sc2::ABILITY_ID::BUILD_TECHLAB || orders[0].ability_id == sc2::ABILITY_ID::BUILD_REACTOR)
						{
							idleProductionBuilding = true;
							break;
						}
					}
				}
			}
#endif

			if (!idleProductionBuilding)
			{
				auto data = m_bot.Data(currentItem.type);
				// if we can make the current item
				m_bot.StartProfiling("2.2.2     tryingToBuild");
				if (meetsReservedResources(currentItem.type, additionalReservedMineral, additionalReservedGas))
				{
					m_bot.StartProfiling("2.2.3     Build without premovement");
					Unit producer = getProducer(currentItem.type);
					// build supply if we need some (SupplyBlock)
					if (producer.isValid())
					{
						if (m_bot.Data(currentItem.type.getUnitType()).supplyCost > m_bot.GetMaxSupply() - m_bot.GetCurrentSupply())
						{
							supplyBlockedFrames++;
#if _DEBUG
							Util::DisplayError("Supply blocked. ", "0x00000007", m_bot);
#else
							Util::Log(__FUNCTION__, "Supply blocked | 0x00000007", m_bot);
#endif
						}

						if (canMakeNow(producer, currentItem.type))
						{
							// create it and remove it from the _queue
							if (create(producer, currentItem, Util::GetTilePosition(m_bot.GetStartLocation())))
							{
								m_queue.removeCurrentHighestPriorityItem();

								// don't actually loop around in here
								m_bot.StopProfiling("2.2.2     tryingToBuild");
								m_bot.StopProfiling("2.2.3     Build without premovement");
								break;
							}
						}
					}
					m_bot.StopProfiling("2.2.3     Build without premovement");
				}
				else if (data.isBuilding
					&& !data.isAddon
					&& !currentItem.type.getUnitType().isMorphedBuilding()
					&& !data.isResourceDepot)//TODO temporary until we have a better solution, allow this if the enemy base doesn't look aggressive
				{
					// is a building (doesn't include addons, because no travel time) and we can make it soon (canMakeSoon)

					m_bot.StartProfiling("2.2.4     Build with premovement");
					Building b(currentItem.type.getUnitType(), m_bot.GetBuildingArea());
					//Get building location

					m_bot.StartProfiling("2.2.5     getNextBuildingLocation");
					const CCTilePosition targetLocation = m_bot.Buildings().getNextBuildingLocation(b, true, true);
					m_bot.StopProfiling("2.2.5     getNextBuildingLocation");
					if (targetLocation != CCTilePosition(0, 0))
					{
						Unit worker = m_bot.Workers().getClosestMineralWorkerTo(Util::GetPosition(targetLocation));
						if (worker.isValid())
						{
							b.finalPosition = targetLocation;
							if (canMakeAtArrival(b, worker, additionalReservedMineral, additionalReservedGas))
							{
								// create it and remove it from the _queue
								if (create(worker, b) && worker.isValid())
								{
									worker.move(targetLocation);
									m_queue.removeCurrentHighestPriorityItem();
								}

								// don't actually loop around in here
								m_bot.StopProfiling("2.2.2     tryingToBuild");
								m_bot.StopProfiling("2.2.4     Build with premovement");
								break;
							}
						}
					}
					else
					{
						Util::DisplayError("Invalid build location for " + currentItem.type.getName(), "0x0000002", m_bot);
					}
					m_bot.StopProfiling("2.2.4     Build with premovement");
				}
				m_bot.StopProfiling("2.2.2     tryingToBuild");
			}
		}
		              
    	// if we can't skip the current item, we stop here
		if (!m_queue.canSkipItem())
			break;

        // otherwise, skip it
        m_queue.skipItem();

        // and get the next one
        currentItem = m_queue.getNextHighestPriorityItem();
    }
	m_bot.StopProfiling("2.2   checkQueue");
}

void ProductionManager::putImportantBuildOrderItemsInQueue()
{
	if (m_bot.Config().AllowDebug && m_bot.GetCurrentFrame() % 10)
		return;

	const float productionScore = getProductionScore();
	const auto productionBuildingCount = getProductionBuildingsCount();
	const auto productionBuildingAddonCount = getProductionBuildingsAddonsCount();
	const auto baseCount = m_bot.Bases().getBaseCount(Players::Self, true);
	const int bansheeCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Banshee.getUnitType(), false, true);
	const int starportCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Starport.getUnitType(), false, true);

	const int currentStrategy = m_bot.Strategy().getCurrentStrategyPostBuildOrder();

	// build supply if we need some
	const auto supplyWithAdditionalSupplyDepot = m_bot.GetMaxSupply() + m_bot.Buildings().countBeingBuilt(supplyProvider) * 8;
	if(m_bot.GetCurrentSupply() + 1.65 * getUnitTrainingBuildings(m_bot.GetSelfRace()).size() + baseCount > supplyWithAdditionalSupplyDepot
		&& !m_queue.contains(supplyProviderType)
		&& supplyWithAdditionalSupplyDepot < 200
		&& !m_bot.Strategy().isWorkerRushed())
	{
		m_queue.queueAsHighestPriority(supplyProviderType, false);
	}

	if (m_bot.GetSelfRace() == sc2::Race::Terran)
	{
		// Logic for building Orbital Commands and Refineries
		const size_t ccCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::CommandCenter.getUnitType(), false, true);
		const size_t completedCCCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::CommandCenter.getUnitType(), true, true);
		if(completedCCCount > 0)
		{
			if (!m_queue.contains(MetaTypeEnum::OrbitalCommand) && !m_queue.contains(MetaTypeEnum::PlanetaryFortress))
			{
				const size_t orbitalCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::OrbitalCommand.getUnitType(), false, true);
				if (orbitalCount < 3)
				{
					m_queue.queueAsHighestPriority(MetaTypeEnum::OrbitalCommand, false);
				}
				else
				{
					m_queue.queueAsHighestPriority(MetaTypeEnum::PlanetaryFortress, false);
				}
			}
		}
		// We want to wait for our first Banshee to build our second CC, otherwise we might have difficulty defending it **COMMENTED**
		else if (ccCount == 0 && !m_queue.contains(MetaTypeEnum::CommandCenter) && starportCount > 0)
		{
			m_queue.queueAsLowestPriority(MetaTypeEnum::CommandCenter, false);
		}

		// Strategy base logic
		switch (currentStrategy)
		{
			case StrategyPostBuildOrder::TERRAN_REAPER :
			{
				//if (productionScore < (float)baseCount)
				{
					if (productionBuildingAddonCount < productionBuildingCount)
					{//Addon
						bool hasPicked = false;
						MetaType toBuild;
						const int starportAddonCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::StarportTechLab.getUnitType(), false, true) +
							m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::StarportReactor.getUnitType(), false, true);
						if (starportCount > starportAddonCount)
						{
							toBuild = MetaTypeEnum::StarportTechLab;
							hasPicked = true;
						}

						if (hasPicked && !m_queue.contains(toBuild))
						{
							m_queue.queueItem(BuildOrderItem(toBuild, 1, false));
						}
					}
					//Building
					bool hasPicked = false;
					MetaType toBuild;
					const int barracksCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Barracks.getUnitType(), false, true);
					const int factoryCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Factory.getUnitType(), false, true);
					if (barracksCount < 1)
					{
						toBuild = MetaTypeEnum::Barracks;
						hasPicked = true;
					}
					else if (m_bot.Strategy().enemyHasMetabolicBoost() && factoryCount * 2 < baseCount)
					{
						toBuild = MetaTypeEnum::Factory;
						hasPicked = true;
					}
					else if (starportCount < baseCount * (m_bot.Strategy().enemyHasMetabolicBoost() ? 1 : 2))
					{
						toBuild = MetaTypeEnum::Starport;
						hasPicked = true;
					}

					if (hasPicked && !m_queue.contains(toBuild) && !m_queue.contains(MetaTypeEnum::CommandCenter))
					{
						m_queue.queueAsLowestPriority(toBuild, false);
					}
				}

				if (!isTechQueuedOrStarted(MetaTypeEnum::BansheeCloak) && !m_bot.Strategy().isUpgradeCompleted(sc2::UPGRADE_ID::BANSHEECLOAK))
				{
					queueTech(MetaTypeEnum::BansheeCloak);
				}

				if (!isTechQueuedOrStarted(MetaTypeEnum::HyperflightRotors) && bansheeCount > 0 && !m_bot.Strategy().isUpgradeCompleted(sc2::UPGRADE_ID::BANSHEESPEED))
				{
					queueTech(MetaTypeEnum::HyperflightRotors);
				}

#ifndef NO_UNITS
				if (!m_bot.Strategy().enemyHasMetabolicBoost() && !m_queue.contains(MetaTypeEnum::Reaper) && m_bot.CombatAnalyzer().GetRatio(sc2::UNIT_TYPEID::TERRAN_REAPER) > 3)
				{
					m_queue.queueItem(BuildOrderItem(MetaTypeEnum::Reaper, 0, false));
				}
#endif

				const int vikingCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Viking.getUnitType(), false, true);

				/*int reaperCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Reaper.getUnitType(), false, true);
				if (reaperCount > 3)
				{
					auto metaTypeInfantryWeapon = queueUpgrade(MetaTypeEnum::TerranInfantryWeaponsLevel1);
				}*/

#ifndef NO_UNITS
				if (!m_queue.contains(MetaTypeEnum::Banshee))
				{
					m_queue.queueItem(BuildOrderItem(MetaTypeEnum::Banshee, 0, false));
				}
#endif

#ifndef NO_UNITS
				if (bansheeCount >= 2 && m_bot.GetPlayerRace(Players::Enemy) == sc2::Terran && !m_queue.contains(MetaTypeEnum::Raven) && m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Raven.getUnitType(), false, true) < 1)
				{
					m_queue.queueItem(BuildOrderItem(MetaTypeEnum::Raven, 0, false));
				}
#endif

#ifndef NO_UNITS
				if ((m_bot.Strategy().isEarlyRushed() || m_bot.Strategy().enemyHasMetabolicBoost() || m_bot.Strategy().enemyHasMassZerglings()) && !m_queue.contains(MetaTypeEnum::Hellion))
				{
					m_queue.queueItem(BuildOrderItem(MetaTypeEnum::Hellion, 0, false));
				}
#endif

				if(m_bot.Strategy().enemyHasMetabolicBoost())
				{
					const int hellionCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Hellion.getUnitType(), true, true);
					if (hellionCount >= 2 && !isTechQueuedOrStarted(MetaTypeEnum::InfernalPreIgniter) && !m_bot.Strategy().isUpgradeCompleted(sc2::UPGRADE_ID::HIGHCAPACITYBARRELS))
					{
						queueTech(MetaTypeEnum::InfernalPreIgniter);
					}
				}

				if (m_bot.Strategy().shouldProduceAntiAir())
				{
#ifndef NO_UNITS
					if (vikingCount < bansheeCount && !m_queue.contains(MetaTypeEnum::Viking))
					{
						m_queue.queueItem(BuildOrderItem(MetaTypeEnum::Viking, 0, false));
					}
#endif

					if (!isTechQueuedOrStarted(MetaTypeEnum::HiSecAutoTracking) && !m_bot.Strategy().isUpgradeCompleted(sc2::UPGRADE_ID::HISECAUTOTRACKING))
					{
						queueTech(MetaTypeEnum::HiSecAutoTracking);
					}
				}
				else if (m_bot.Strategy().enemyOnlyHasFlyingBuildings())
				{
#ifndef NO_UNITS
					if (vikingCount < 1 && !m_queue.contains(MetaTypeEnum::Viking))
					{
						m_queue.queueItem(BuildOrderItem(MetaTypeEnum::Viking, 0, false));
					}
#endif
				}
				break;
			}
			case StrategyPostBuildOrder::WORKER_RUSH_DEFENSE:
			{
				if (!m_queue.contains(MetaTypeEnum::Reaper))
				{
					m_queue.queueAsHighestPriority(MetaTypeEnum::Reaper, false);
				}

				if (!m_queue.contains(MetaTypeEnum::Refinery) && m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Refinery.getUnitType(), false, false) == 0)
				{
					m_queue.queueAsHighestPriority(MetaTypeEnum::Refinery, false);
				}
				return;
			}
			case StrategyPostBuildOrder::NO_STRATEGY:
				break;
			default:
			{
				assert("This strategy doesn't exist.");
			}
		}
	}
	
	if (!m_queue.contains(workerMetatype))//check queue
	{
		const int maxWorkersPerBase = 27;//21 mineral (to prepare for the next expansion), 6 gas
		const int maxWorkers = maxWorkersPerBase * 3;//maximum of 3 bases.
		const int workerCount = m_bot.Workers().getNumWorkers();
		if (baseCount * maxWorkersPerBase > workerCount && workerCount < maxWorkers)
		{
			if (currentStrategy != StrategyPostBuildOrder::WORKER_RUSH_DEFENSE)//check strategy
			{
				m_queue.queueItem(BuildOrderItem(workerMetatype, 2, false));
			}
		}
	}
}

void ProductionManager::QueueDeadBuildings()
{
	std::vector<Unit> deadBuildings;
	auto baseBuildings = m_bot.Buildings().getBaseBuildings();
	auto previousBuildings = m_bot.Buildings().getPreviousBaseBuildings();
	for (int i = 0; i < previousBuildings.size(); i++)
	{
		auto unit = previousBuildings.at(i);
		auto it = std::find(baseBuildings.begin(), baseBuildings.end(), unit);
		if (it == baseBuildings.end())
		{
			deadBuildings.push_back(unit);

			//Manual find of the worker to free him.
			auto & buildings = m_bot.Buildings().getPreviousBuildings();
			auto buildingsIt = buildings.begin();
			for (; buildingsIt != buildings.end(); ++buildingsIt)
			{
				if (buildingsIt->buildingUnit.isValid() && buildingsIt->buildingUnit.getTag() == unit.getTag())
				{
					m_bot.Workers().finishedWithWorker(buildingsIt->builderUnit);
					std::vector<Building> toRemove;
					toRemove.push_back(*buildingsIt);
					m_bot.Buildings().removeBuildings(toRemove);
					break;
				}
			}
		}
	}
	for (int i = 0; i < deadBuildings.size(); i++)
	{
		MetaType type = MetaType(deadBuildings.at(i).getType(), m_bot);
		if (!m_queue.contains(type))
		{
			m_queue.queueItem(BuildOrderItem(type, 0, false));
		}
	}

	m_bot.Buildings().updatePreviousBuildings();
	m_bot.Buildings().updatePreviousBaseBuildings();
}

void ProductionManager::fixBuildOrderDeadlock(BuildOrderItem & item)
{
	const TypeData& typeData = m_bot.Data(item.type);

	// check to see if we have the prerequisites for the item
    if (!hasRequired(item.type, true))
    {
        std::cout << item.type.getName() << " needs a requirement: " << typeData.requiredUnits[0].getName() << "\n";
		BuildOrderItem requiredItem = m_queue.queueItem(BuildOrderItem(MetaType(typeData.requiredUnits[0], m_bot), 0, item.blocking));
        fixBuildOrderDeadlock(requiredItem);
        return;
    }

    // build the producer of the unit if we don't have one
    if (!hasProducer(item.type, true))
    {
        if (item.type.getUnitType().isWorker() && m_bot.Observation()->GetFoodWorkers() == 0)
        {
            // We no longer have worker and no longer have buildings to do more, so we are rip...
            return;
        }
		std::cout << item.type.getName() << " needs a producer: " << typeData.whatBuilds[0].getName() << "\n";
		BuildOrderItem producerItem = m_queue.queueItem(BuildOrderItem(MetaType(typeData.whatBuilds[0], m_bot), 0, item.blocking));
        fixBuildOrderDeadlock(producerItem);
    }

    // build a refinery if we don't have one and the thing costs gas
    auto refinery = Util::GetRefinery(m_bot.GetSelfRace(), m_bot);
	if (typeData.gasCost > 0 && m_bot.UnitInfo().getUnitTypeCount(Players::Self, refinery, false, true) == 0)
    {
		m_queue.queueAsHighestPriority(MetaType(refinery, m_bot), true);
    }
}

void ProductionManager::lowPriorityChecks()
{
	if (m_bot.GetGameLoop() % 10)
	{
		return;
	}

	// build a refinery if we are missing one
	//TODO doesn't handle extra hatcheries, doesn't handle rich geyser
	auto refinery = Util::GetRefinery(m_bot.GetSelfRace(), m_bot);
	if (m_bot.Workers().canHandleMoreRefinery() && !m_queue.contains(MetaType(refinery, m_bot)))
	{
		if (m_initialBuildOrderFinished && !m_bot.Strategy().isWorkerRushed())
		{
			auto baseCount = m_bot.Bases().getBaseCount(Players::Self, true);
			auto geyserCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, refinery, false, true);
			if (geyserCount < baseCount * 2)
			{
				m_queue.queueAsLowestPriority(MetaType(refinery, m_bot), false);
			}
		}
	}

	//build turrets in mineral field
	//TODO only supports terran, turret position isn't always optimal(check BaseLocation to optimize it)
	bool shouldProduceAntiAir = m_bot.Strategy().shouldProduceAntiAir();
	if (shouldProduceAntiAir || m_bot.Strategy().enemyHasInvisible())
	{
		auto engeneeringBayCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::EngineeringBay.getUnitType(), false, true);
		if (engeneeringBayCount <= 0 && !m_queue.contains(MetaTypeEnum::EngineeringBay))
		{
			const int starportCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Starport.getUnitType(), false, true);
			if (starportCount > 0)
			{
				const int vikingCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::Viking.getUnitType(), false, true);
				if (vikingCount == 0)
				{
					m_queue.queueAsHighestPriority(MetaTypeEnum::Viking, false);
				}
				m_queue.queueAsLowestPriority(MetaTypeEnum::EngineeringBay, false);
			}
			else
			{
				m_queue.queueAsHighestPriority(MetaTypeEnum::EngineeringBay, false);
			}
		}

		if (!m_bot.Buildings().isConstructingType(MetaTypeEnum::MissileTurret.getUnitType()))
		{
			int engeneeringCount = m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::EngineeringBay.getUnitType(), true, true);
			if (engeneeringCount > 0)
			{
				for (auto base : m_bot.Bases().getOccupiedBaseLocations(Players::Self))
				{
					auto hasTurret = false;
					auto position = base->getTurretPosition();
					auto buildings = m_bot.Buildings().getFinishedBuildings();
					for (auto b : buildings)
					{
						if (b.getTilePosition() == position)
						{
							hasTurret = true;
							break;
						}
					}
					if (!hasTurret)
					{
						m_bot.Buildings().getBuildingPlacer().freeTilesForTurrets(position);
						auto worker = m_bot.Workers().getClosestMineralWorkerTo(CCPosition(position.x, position.y));
						create(worker, BuildOrderItem(MetaTypeEnum::MissileTurret, 0, false), position);
					}
				}
			}
		}
	}
}

bool ProductionManager::currentlyHasRequirement(MetaType currentItem) const
{
 	auto requiredUnits = m_bot.Data(currentItem).requiredUnits;
	if (requiredUnits.empty())
	{
		return true;
	}

	for (auto & required : m_bot.Data(currentItem).requiredUnits)
	{
		
		if (m_bot.UnitInfo().getUnitTypeCount(Players::Self, required, true, true) <= 0)
		{
			//Only for terran because all their bases are used for the same prerequirements. Not the case for zergs.
			//TODO zerg might need something similar
			sc2::UNIT_TYPEID type = required.getAPIUnitType();
			switch (type)
			{
				case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
				case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTERFLYING:
				case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
				case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMANDFLYING:
				case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
				{
					return m_bot.Bases().getBaseCount(Players::Self) > 0;
					break;
				}
				case sc2::UNIT_TYPEID::TERRAN_TECHLAB:
				{//TODO Techlabs may all be considereed the same, should verify and fix if it is the case.
					switch ((sc2::UNIT_TYPEID)currentItem.getUnitType().getAPIUnitType())
					{
						case sc2::UNIT_TYPEID::TERRAN_MARAUDER:
						case sc2::UNIT_TYPEID::TERRAN_GHOST:
						{
							return m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::BarracksTechLab.getUnitType(), true, true) >= 0;
						}
						case sc2::UNIT_TYPEID::TERRAN_SIEGETANK:
						case sc2::UNIT_TYPEID::TERRAN_THOR:
						{
							return m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::FactoryTechLab.getUnitType(), true, true) >= 0;
						}
						case sc2::UNIT_TYPEID::TERRAN_RAVEN:
						case sc2::UNIT_TYPEID::TERRAN_BANSHEE:
						case sc2::UNIT_TYPEID::TERRAN_BATTLECRUISER:
						{
							return m_bot.UnitInfo().getUnitTypeCount(Players::Self, MetaTypeEnum::StarportTechLab.getUnitType(), true, true) >= 0;
						}
					}
				}
				default:
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool ProductionManager::hasRequired(const MetaType& metaType, bool checkInQueue)
{
	const TypeData& typeData = m_bot.Data(metaType);

	if (typeData.requiredUnits.empty())
		return true;

	for (auto & required : typeData.requiredUnits)
	{
		if (m_bot.UnitInfo().getUnitTypeCount(Players::Self, required, false, true) > 0 || m_bot.Buildings().isBeingBuilt(required))
			return true;

		if (checkInQueue && m_queue.contains(MetaType(required, m_bot)))
			return true;
	}

	return false;
}

bool ProductionManager::hasProducer(const MetaType& metaType, bool checkInQueue)
{
	const TypeData& typeData = m_bot.Data(metaType);

	if (typeData.whatBuilds.empty())
		return true;

	for (auto & producer : typeData.whatBuilds)
	{
		if (metaType.getUnitType().isWorker() && m_bot.Bases().getBaseCount(Players::Self) > 0)
			return true;
		if (metaType.isAddon())
		{
			int count = m_bot.UnitInfo().getUnitTypeCount(Players::Self, producer, false, false);
			auto buildings = m_bot.Buildings().getBaseBuildings();
			for (auto building : buildings)
			{
				if (building.getType() == producer)
				{
					if (building.getAddonTag() == 0)
					{
						return true;
					}
					--count;
				}
			}
			if (count > 0)	// This hack is because there is a moment where the building is purchased and removed from the queue but not yet in the buildings manager
				return true;
			
			//Addons do not check further or else we could try to build an addon on a building with an addon already.
			return checkInQueue && m_queue.contains(MetaType(producer, m_bot));
		}
		if (m_bot.UnitInfo().getUnitTypeCount(Players::Self, producer, false, true) > 0)
			return true;
		if (m_bot.Buildings().isBeingBuilt(producer))
			return true;
		if (checkInQueue && m_queue.contains(MetaType(producer, m_bot)))
			return true;
	}

	return false;
}

Unit ProductionManager::getProducer(const MetaType & type, CCPosition closestTo) const
{
	// get all the types of units that cna build this type
	auto & producerTypes = m_bot.Data(type).whatBuilds;
	bool priorizeReactor = false;
	bool isTypeAddon = m_bot.Data(type).isAddon;

	if (m_bot.GetSelfRace() == CCRace::Terran)
	{
		//check if we can prioritize a building with a reactor instead of a techlab
		auto typeName = type.getName();
		if (typeName == "Marine" ||
			typeName == "Reaper" ||
			typeName == "WidowMine" |
			typeName == "Hellion" ||
			typeName == "VikingFighter" ||
			typeName == "Medivac" ||
			typeName == "Liberator")
		{

			for (auto & producerType : producerTypes)
			{
				for (auto & unit : m_bot.GetAllyUnits(producerType.getAPIUnitType()))
				{
					// reasons a unit can not train the desired type
					if (!unit.isValid()) { continue; }
					if (!unit.isCompleted()) { continue; }
					if (unit.isFlying()) { continue; }
					if (unit.isAddonTraining()) { continue; }

					//Building can produce unit, now check if addon is reactor and available
					auto addonTag = unit.getAddonTag();
					if (addonTag == 0)
					{
						continue;
					}

					auto addon = m_bot.GetUnit(addonTag);
					auto addonType = (sc2::UNIT_TYPEID)addon.getAPIUnitType();
					switch (addonType)
					{
						case sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR:
						case sc2::UNIT_TYPEID::TERRAN_FACTORYREACTOR:
						case sc2::UNIT_TYPEID::TERRAN_STARPORTREACTOR:
						{
							priorizeReactor = true;
							break;
						}
					}
					break;
				}
			}
		}
	}

    // make a set of all candidate producers
    std::vector<Unit> candidateProducers;
	for (auto & producerType : producerTypes)
	{
		for (auto & unit : m_bot.GetAllyUnits(producerType.getAPIUnitType()))
		{
			// reasons a unit can not train the desired type
			if (!unit.isValid()) { continue; }
			if (!unit.isCompleted()) { continue; }
			if (unit.isFlying()) { continue; }

			bool isBuilding = m_bot.Data(unit).isBuilding;
			if (isBuilding && unit.isTraining() && unit.getAddonTag() == 0) { continue; }//TODO might break other races
			if (isBuilding && m_bot.GetSelfRace() == CCRace::Terran)
			{//If is terran, check for Reactor addon
				sc2::Tag addonTag = unit.getAddonTag();
				sc2::UNIT_TYPEID unitType = unit.getAPIUnitType();

				if (addonTag != 0)
				{
					bool addonIsReactor = false;
					auto addon = m_bot.GetUnit(addonTag);
					switch ((sc2::UNIT_TYPEID)addon.getAPIUnitType())
					{
					case sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR:
					case sc2::UNIT_TYPEID::TERRAN_FACTORYREACTOR:
					case sc2::UNIT_TYPEID::TERRAN_STARPORTREACTOR:
					{
						addonIsReactor = true;
						break;
					}
					}

					if (unit.isTraining() && !addonIsReactor)
					{//skip, Techlab can't build two units
						continue;
					}

					if (addonIsReactor && unit.isAddonTraining())
					{//skip, reactor at max capacity
						continue;
					}

					//Skip techlab if we have an available reactor
					if (priorizeReactor && !addonIsReactor)
					{
						continue;
					}
				}
			}

			switch ((sc2::UNIT_TYPEID)unit.getAPIUnitType())
			{
			case sc2::UNIT_TYPEID::TERRAN_SCV:
			case sc2::UNIT_TYPEID::PROTOSS_PROBE:
			case sc2::UNIT_TYPEID::ZERG_DRONE:
			{
				if (!m_bot.Workers().isFree(unit))
				{
					continue;
				}
				break;
			}
			case sc2::UNIT_TYPEID::TERRAN_MULE:
			{
				continue;
			}
			}

			// TODO: if unit is not powered continue
			// TODO: if the type requires an addon and the producer doesn't have one

			// if the type we want to produce has required units, we make sure the unit is one of those unit types
			if (m_bot.Data(type).requiredUnits.size() > 0)
			{
				bool hasRequiredUnit = false;
				for (UnitType requiredUnit : m_bot.Data(type).requiredUnits)
				{
					if (!requiredUnit.isAddon())
					{
						// maybe we don't hve what is needed, but it seems to already work for non addon units
						hasRequiredUnit = true;
						break;
					}
					else	// addon
					{
						if (unit.getUnitPtr()->add_on_tag != 0)
						{
							Unit addon = m_bot.GetUnit(unit.getUnitPtr()->add_on_tag);
							if (requiredUnit.getAPIUnitType() == addon.getAPIUnitType())
							{
								hasRequiredUnit = true;
								break;
							}
						}
					}
				}
				if (!hasRequiredUnit) { continue; }
			}

			//if the type is an addon, some special cases
			if (isTypeAddon)
			{
				//Skip if the building already has an addon
				if (unit.getUnitPtr()->add_on_tag != 0)
				{
					continue;
				}
			}

			// if we haven't cut it, add it to the set of candidates
			candidateProducers.push_back(unit);
		}
	}

    return getClosestUnitToPosition(candidateProducers, closestTo);
}

std::vector<sc2::UNIT_TYPEID> ProductionManager::getProductionBuildingTypes() const
{
	switch (m_bot.GetSelfRace())
	{
	case CCRace::Terran:
	{
		return {sc2::UNIT_TYPEID::TERRAN_BARRACKS,
			sc2::UNIT_TYPEID::TERRAN_BARRACKSFLYING,
			sc2::UNIT_TYPEID::TERRAN_FACTORY,
			sc2::UNIT_TYPEID::TERRAN_FACTORYFLYING,
			sc2::UNIT_TYPEID::TERRAN_STARPORT,
			sc2::UNIT_TYPEID::TERRAN_STARPORTFLYING };
	}
	case CCRace::Protoss:
	{
		//TODO
		return {};
	}
	case CCRace::Zerg:
	{
		//TODO
		return {};
	}
	}
	return {};
}

int ProductionManager::getProductionBuildingsCount() const
{
	auto productionBuildingTypes = getProductionBuildingTypes();
	return m_bot.Buildings().getBuildingCountOfType(productionBuildingTypes);
}

int ProductionManager::getProductionBuildingsAddonsCount() const
{
	switch (m_bot.GetSelfRace())
	{
		case CCRace::Terran:
		{
			std::vector<sc2::UNIT_TYPEID> addonTypes = {
				sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR,
				sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB,
				sc2::UNIT_TYPEID::TERRAN_FACTORYREACTOR,
				sc2::UNIT_TYPEID::TERRAN_FACTORYTECHLAB,
				sc2::UNIT_TYPEID::TERRAN_STARPORTREACTOR,
				sc2::UNIT_TYPEID::TERRAN_STARPORTTECHLAB };
			return m_bot.Buildings().getBuildingCountOfType(addonTypes);
		}
		case CCRace::Protoss:
		{
			return 0;
		}
		case CCRace::Zerg:
		{
			return 0;
		}
	}
	return 0;
}

float ProductionManager::getProductionScore() const
{
	float score = 0;
	switch (m_bot.GetSelfRace())
	{
		case CCRace::Terran:
		{
			score += m_bot.Buildings().getBuildingCountOfType({
				sc2::UNIT_TYPEID::TERRAN_BARRACKS }) * 0.25f;
			score += m_bot.Buildings().getBuildingCountOfType({
				sc2::UNIT_TYPEID::TERRAN_FACTORY }) * 0.25f;
			score += m_bot.Buildings().getBuildingCountOfType({
				sc2::UNIT_TYPEID::TERRAN_STARPORT }) * 0.5f;

			std::vector<sc2::UNIT_TYPEID> barracksAddonTypes = {
				sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR,
				sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB };
			score += m_bot.Buildings().getBuildingCountOfType(barracksAddonTypes) * 0.25f;
			std::vector<sc2::UNIT_TYPEID> factoryAddonTypes = {
				sc2::UNIT_TYPEID::TERRAN_FACTORYREACTOR,
				sc2::UNIT_TYPEID::TERRAN_FACTORYTECHLAB };
			score += m_bot.Buildings().getBuildingCountOfType(factoryAddonTypes) * 0.25f;
			std::vector<sc2::UNIT_TYPEID> starportAddonTypes = {
				sc2::UNIT_TYPEID::TERRAN_STARPORTREACTOR,
				sc2::UNIT_TYPEID::TERRAN_STARPORTTECHLAB };
			score += m_bot.Buildings().getBuildingCountOfType(starportAddonTypes) * 0.5f;
			break;
		}
		case CCRace::Protoss:
		{
			return 0;
		}
		case CCRace::Zerg:
		{
			return 0;
		}
	}
	return score;
}

float ProductionManager::getProductionScoreInQueue()
{
	float score = 0;
	
	switch (m_bot.GetSelfRace())
	{
		case CCRace::Terran:
		{
			score += m_queue.contains(MetaTypeEnum::Barracks) ? 0.25f : 0;
			score += m_queue.contains(MetaTypeEnum::Factory) ? 0.25f : 0;
			score += m_queue.contains(MetaTypeEnum::Starport) ? 0.25f : 0;

			score += m_queue.contains(MetaTypeEnum::BarracksReactor) ? 0.25f : 0;
			score += m_queue.contains(MetaTypeEnum::BarracksTechLab) ? 0.25f : 0;
			score += m_queue.contains(MetaTypeEnum::FactoryReactor) ? 0.25f : 0;
			score += m_queue.contains(MetaTypeEnum::FactoryTechLab) ? 0.25f : 0;
			score += m_queue.contains(MetaTypeEnum::StarportReactor) ? 0.25f : 0;
			score += m_queue.contains(MetaTypeEnum::StarportTechLab) ? 0.25f : 0;
			break;
		}
		case CCRace::Protoss:
		{
			return 0;
		}
		case CCRace::Zerg:
		{
			return 0;
		}
	}
	return score;
}

std::vector<Unit> ProductionManager::getUnitTrainingBuildings(CCRace race)
{
	std::set<sc2::UnitTypeID> unitTrainingBuildingTypes;
	switch (race)
	{
		case CCRace::Terran:
			unitTrainingBuildingTypes.insert(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
			unitTrainingBuildingTypes.insert(sc2::UNIT_TYPEID::TERRAN_STARPORT);
			unitTrainingBuildingTypes.insert(sc2::UNIT_TYPEID::TERRAN_FACTORY);
			break;
		case CCRace::Protoss:
			//TODO complete
			break;
		case CCRace::Zerg:
			//TODO complete
			break;
	}

	std::vector<Unit> trainers;
	for (auto unit : m_bot.UnitInfo().getUnits(Players::Self))
	{
		// reasons a unit can not train the desired type
		if (unitTrainingBuildingTypes.find(unit.getType().getAPIUnitType()) == unitTrainingBuildingTypes.end()) { continue; }
		if (!unit.isCompleted()) { continue; }
		if (unit.isFlying()) { continue; }

		// TODO: if unit is not powered continue

		// if we haven't cut it, add it to the set of trainers
		trainers.push_back(unit);
	}

	return trainers;
}

bool ProductionManager::queueUpgrade(const MetaType & type, bool balanceUpgrades, bool ifFinishedTryHigherLevel)
{
	assert(true, "deprecated");

	MetaType previousUpgradeName;
	auto upgradeName = type;
	bool started = false;
	bool categoryFound = false;

	//If upgrade is currently queued, we don't have to queue anything
	if (m_queue.contains(upgradeName))
	{
		return false;
	}
	for (const auto & incompletUpgrade : incompletUpgradesMetatypes)//If startedUpgrades.contains
	{
		if (incompletUpgrade == upgradeName)
		{
			return false;
		}
	}

	std::list<MetaType> startedOrFinished = incompletUpgradesMetatypes;
	//Merge completUpgrades into startOrFinished because list.merge empties the second list for no reason
	for (auto & completUpgrade : completUpgrades)
	{
		startedOrFinished.push_back(completUpgrade);
	}

	for (auto & upCategory : possibleUpgrades)
	{
		for (auto & potentialUpgrade : upCategory)
		{
			if (started)
			{
				//if upgrade is already being research, just cancel.
				if (m_queue.contains(potentialUpgrade))
				{
					return false;
				}
				if (std::find(incompletUpgradesMetatypes.begin(), incompletUpgradesMetatypes.end(), potentialUpgrade) != incompletUpgradesMetatypes.end())
				{
					return false;
				}
				previousUpgradeName = upgradeName;
				upgradeName = potentialUpgrade;
				started = false;
			}
			if (potentialUpgrade == upgradeName)
			{
				categoryFound = true;

				if (m_queue.contains(potentialUpgrade))
				{
					return false;
				}
				for (const auto & startedUpgrade : startedOrFinished)//If startedUpgrades.contains
				{
					if (startedUpgrade == upgradeName)
					{
						if (balanceUpgrades)
						{
							auto alternateUpgrade = alternateUpgrades[type.getName()];
							if (queueUpgrade(alternateUpgrade, false, false))
							{
								return true;
							}

							//if we didn't queue it, but it is not completed, it has to be in progress so we don't queue anything.
							if (std::find(completUpgrades.begin(), completUpgrades.end(), alternateUpgrade) == completUpgrades.end())
							{
								return false;
							}
						}
						if (ifFinishedTryHigherLevel)
						{
							started = true;
							previousUpgradeName = potentialUpgrade;
							break;
						}
						return false;
					}
				}
				if (!started)//if not started, start it.
				{
					//Can't merge both if since [empty] isn't a MetaType.
					bool isPreviousUpgradeViable = previousUpgradeName.getName() != "MetaType" && !isTechQueuedOrStarted(previousUpgradeName);
					if (isPreviousUpgradeViable	|| (!isPreviousUpgradeViable && !isTechQueuedOrStarted(potentialUpgrade)))
					{
						queueTech(potentialUpgrade);
						return true;
					}
					//Did not finish previous upgrade.
					break;
				}
				//if started, return the next one.
			}
		}
		if (categoryFound)
		{
			//Finished everything in the category
			break;
		}
	}
	return false;
}

bool ProductionManager::isTechQueuedOrStarted(const MetaType & type)
{
	return std::find(incompletUpgradesMetatypes.begin(), incompletUpgradesMetatypes.end(), type) != incompletUpgradesMetatypes.end()
		|| std::find(completUpgrades.begin(), completUpgrades.end(), type) != completUpgrades.end()
		|| m_queue.contains(type);
}

bool ProductionManager::isTechStarted(const MetaType & type)
{
	return std::find(incompletUpgradesMetatypes.begin(), incompletUpgradesMetatypes.end(), type) != incompletUpgradesMetatypes.end()
		|| std::find(completUpgrades.begin(), completUpgrades.end(), type) != completUpgrades.end();
}

bool ProductionManager::isTechFinished(const MetaType & type)
{
	return std::find(completUpgrades.begin(), completUpgrades.end(), type) != completUpgrades.end();
}

void ProductionManager::queueTech(const MetaType & type)
{
	m_queue.queueItem(BuildOrderItem(type, 0, false));
	Util::DebugLog(__FUNCTION__, "Queue " + type.getName(), m_bot);
}

void ProductionManager::validateUpgradesProgress()
{
	if(incompletUpgrades.empty())
	{
		return;
	}

	std::vector<MetaType> toRemove;
	for (std::pair<const MetaType, Unit> & upgrade : incompletUpgrades)
	{
		bool found = false;
		float progress = 0.f;
		auto unitPtr = upgrade.second.getUnitPtr();
		
		switch ((sc2::UNIT_TYPEID)upgrade.second.getType().getAPIUnitType())
		{
			case sc2::UNIT_TYPEID::TERRAN_ARMORY:
			case sc2::UNIT_TYPEID::TERRAN_ENGINEERINGBAY:
			case sc2::UNIT_TYPEID::PROTOSS_FORGE:
			case sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE:
			case sc2::UNIT_TYPEID::ZERG_EVOLUTIONCHAMBER:
			case sc2::UNIT_TYPEID::ZERG_SPIRE:
			case sc2::UNIT_TYPEID::ZERG_GREATERSPIRE:
				found = true;// Skip because the buildAbility is, for example, RESEARCH_TERRANSHIPWEAPONS = 3699 instead of RESEARCH_TERRANSHIPWEAPONSLEVEL1 = 861
				if (!unitPtr->orders.empty())
				{
					progress = unitPtr->orders.at(0).progress;
				}
				break;

			default:
				auto buildAbilityId = m_bot.Data(upgrade.first.getUpgrade()).buildAbility;
				for (auto & order : unitPtr->orders)
				{
					if (order.ability_id == buildAbilityId)
					{
						found = true;
						progress = order.progress;
						break;
					}
				}
		}

		if (found)
		{
			//check if upgrade is no longer progressing (cancelled)
			if (incompletUpgradesProgress.at(upgrade.first) >= progress)
			{
				toRemove.push_back(upgrade.first);
				Util::DebugLog(__FUNCTION__, "upgrade canceled " + upgrade.first.getName(), m_bot);
			}
			else if (progress > 0.99f)//About to finish, lets consider it done.
			{
				toRemove.push_back(upgrade.first);
				completUpgrades.push_back(upgrade.first);
				Util::DebugLog(__FUNCTION__, "upgrade finished " + upgrade.first.getName(), m_bot);
			}
			else
			{
				incompletUpgradesProgress.at(upgrade.first) = progress;
			}
		}
		else
		{
			toRemove.push_back(upgrade.first);
			Util::DebugLog(__FUNCTION__, "upgrade failed to start " + upgrade.first.getName(), m_bot);
		}
	}
	for (auto & remove : toRemove)
	{
		incompletUpgrades.erase(remove);
		incompletUpgradesMetatypes.remove(remove);
		incompletUpgradesProgress.erase(remove);
	}
}

Unit ProductionManager::getClosestUnitToPosition(const std::vector<Unit> & units, CCPosition closestTo) const
{
    if (units.empty())
    {
        return {};
    }

    // if we don't care where the unit is return the first one we have
    if (closestTo.x == 0 && closestTo.y == 0)
    {
        return units[0];
    }

    Unit closestUnit;
    double minDist = std::numeric_limits<double>::max();

    for (auto & unit : units)
    {
        double distance = Util::DistSq(unit, closestTo);
        if (!closestUnit.isValid() || distance < minDist)
        {
            closestUnit = unit;
            minDist = distance;
        }
    }

    return closestUnit;
}

// this function will check to see if all preconditions are met and then create a unit
// Used to create unit/tech/buildings (when we have the ressources)
bool ProductionManager::create(const Unit & producer, BuildOrderItem & item, CCTilePosition desidredPosition)
{
	if (!producer.isValid())
	{
		return false;
	}

	bool result;
	// if we're dealing with a building
	if (item.type.isBuilding())
	{
		if (item.type.getUnitType().isMorphedBuilding())
		{
			producer.morph(item.type.getUnitType());
			result = true;
		}
		else
		{
			Building b(item.type.getUnitType(), desidredPosition);
			result = m_bot.Buildings().addBuildingTask(b);
		}
	}
	// if we're dealing with a non-building unit
	else if (item.type.isUnit())
	{
		producer.train(item.type.getUnitType());
		result = true;
	}
	else if (item.type.isUpgrade())
	{
		Micro::SmartAbility(producer.getUnitPtr(), m_bot.Data(item.type.getUpgrade()).buildAbility, m_bot);

#if _DEBUG
		if (isTechStarted(item.type))
		{
			Util::DisplayError("Trying to start an already started upgrade.", "0x00000006", m_bot);
		}
#endif
		incompletUpgrades.insert(std::make_pair(item.type, producer));
		incompletUpgradesMetatypes.push_back(item.type);
		incompletUpgradesProgress.insert(std::make_pair(item.type, 0.f));
		Util::DebugLog(__FUNCTION__, "upgrade starting " + item.type.getName(), m_bot);
		result = true;
	}

	return result;
}

// this function will check to see if all preconditions are met and then create a unit
// Used for premove
bool ProductionManager::create(const Unit & producer, Building & b)
{
    if (!producer.isValid())
    {
        return false;
    }

    if (b.type.isMorphedBuilding())
    {
        producer.morph(b.type);
		return true;
    }

	return m_bot.Buildings().addBuildingTask(b);
}

bool ProductionManager::canMakeNow(const Unit & producer, const MetaType & type)
{
    if (!producer.isValid())
    {
        return false;
    }
	if (m_bot.Data(type).isAddon)
	{//Do not check for abilities, buildings don't seem to have it listed. They always have the ability anyway, so its safe.
		return true;
	}

#ifdef SC2API
	sc2::AvailableAbilities available_abilities = m_bot.Query()->GetAbilitiesForUnit(producer.getUnitPtr());

	// quick check if the unit can't do anything it certainly can't build the thing we want
	if (available_abilities.abilities.empty())
	{
		Util::DisplayError("Producer of type " + producer.getType().getName() + " cannot activate its ability to produce " + type.getName(), "0x10000000", m_bot, true);
		return false;
	}
	else
	{
		// check to see if one of the unit's available abilities matches the build ability type
		sc2::AbilityID MetaTypeAbility = m_bot.Data(type).buildAbility;
		for (const sc2::AvailableAbility & available_ability : available_abilities.abilities)
		{
			if (available_ability.ability_id == MetaTypeAbility)
			{
				return true;
			}
		}
		if (type.isUpgrade())//TODO Not safe, is a fix for upgrades having the wrong ID
		{
			std::ostringstream oss;
			oss << "Unit can't create upgrade, but should be able. Upgrade: " << type.getName() << " UnitType: " << producer.getType().getName();
			Util::DisplayError(oss.str(), "0x00000005", m_bot);
			return true;
		}
	}
	
	return false;
#else
	bool canMake = meetsReservedResources(type);
	if (canMake)
	{
		if (type.isUnit())
		{
			canMake = BWAPI::Broodwar->canMake(type.getUnitType().getAPIUnitType(), producer.getUnitPtr());
		}
		else if (type.isTech())
		{
			canMake = BWAPI::Broodwar->canResearch(type.getTechType(), producer.getUnitPtr());
		}
		else if (type.isUpgrade())
		{
			canMake = BWAPI::Broodwar->canUpgrade(type.getUpgrade(), producer.getUnitPtr());
		}
		else
		{
			BOT_ASSERT(false, "Unknown type");
		}
	}

	return canMake;
#endif
}

bool ProductionManager::detectBuildOrderDeadlock()
{
    // TODO: detect build order deadlocks here
    return false;
}

int ProductionManager::getExtraMinerals()
{
	int extraMinerals = 0;
	std::set<Unit> workers = m_bot.Workers().getWorkers();
	for (auto w : workers) {
		if (m_bot.Workers().getWorkerData().getWorkerJob(w) == WorkerJobs::Minerals && m_bot.Workers().isReturningCargo(w))
		{ 
			if (w.getAPIUnitType() == sc2::UNIT_TYPEID::TERRAN_MULE)
			{
				extraMinerals += 25;
			}
			else
			{
				extraMinerals += 5;
			}
		}
	}
	return extraMinerals;
}

int ProductionManager::getExtraGas()
{
	int extraGas = 0;
	std::set<Unit> workers = m_bot.Workers().getWorkers();
	for (auto w : workers) {
		if (m_bot.Workers().getWorkerData().getWorkerJob(w) == WorkerJobs::Gas && m_bot.Workers().isReturningCargo(w))
		{
			extraGas += 4;
		}
	}
	return extraGas;
}

// return whether or not we meet resources, including building reserves
bool ProductionManager::meetsReservedResources(const MetaType & type, int additionalReservedMineral, int additionalReservedGas)
{
    return (m_bot.Data(type).mineralCost <= m_bot.GetFreeMinerals() - additionalReservedMineral) && (m_bot.Data(type).gasCost <= m_bot.GetFreeGas() - additionalReservedGas);
}

// return whether or not we meet resources, including building reserves
bool ProductionManager::meetsReservedResourcesWithExtra(const MetaType & type, int additionalMineral, int additionalGas, int additionalReservedMineral, int additionalReservedGas)
{
	assert("Addons cannot use extra ressources", m_bot.Data(type).isAddon);
	return (m_bot.Data(type).mineralCost <= m_bot.GetFreeMinerals() + additionalMineral - additionalReservedMineral) && (m_bot.Data(type).gasCost <= m_bot.GetFreeGas() + additionalGas - additionalReservedGas);
}

bool ProductionManager::canMakeAtArrival(const Building & b, const Unit & worker, int additionalReservedMineral, int additionalReservedGas)
{
	const float mineralRate = m_bot.Observation()->GetScore().score_details.collection_rate_minerals / 60.f / 16.f;
	const float gasRate = m_bot.Observation()->GetScore().score_details.collection_rate_vespene / 60.f / 16.f;

	if (mineralRate == 0 && gasRate == 0)
	{
		return false;
	}

	//float distance = Util::PathFinding::FindOptimalPathDistance(worker.getUnitPtr(), Util::GetPosition(b.finalPosition), false, m_bot);
	float distance = Util::Dist(worker.getPosition(), Util::GetPosition(b.finalPosition));
	const float speed = 2.8125f;//Always the same for workers, Util::getSpeedOfUnit(worker.getUnitPtr(), m_bot);

	auto speedPerFrame = speed / 16.f;
	auto travelFrame = distance / speedPerFrame;
	auto mineralGain = travelFrame * mineralRate;
	auto gasGain = travelFrame * gasRate;

	if (meetsReservedResourcesWithExtra(MetaType(b.type, m_bot), mineralGain, gasGain, additionalReservedMineral, additionalReservedGas))
	{
		return true;
	}
	return false;
}

void ProductionManager::drawProductionInformation()
{
#ifdef PUBLIC_RELEASE
	return;
#endif
    if (!m_bot.Config().DrawProductionInfo)
    {
        return;
    }

    std::stringstream ss;
    ss << "Production Information\n\n";

    ss << m_queue.getQueueInformation() << "\n\n";
	m_bot.Map().drawTextScreen(0.01f, 0.01f, ss.str(), CCColor(255, 255, 0));

	ss.str(std::string());
	ss << "Free Mineral:     " << m_bot.GetFreeMinerals() << "\n";
	ss << "Free Gas:         " << m_bot.GetFreeGas() << "\n";
	ss << "Gas Worker Target:" << m_bot.Workers().getGasWorkersTarget();
	m_bot.Map().drawTextScreen(0.75f, 0.05f, ss.str(), CCColor(255, 255, 0));

	ss.str(std::string());
	ss << "Being built:      \n";
	for (auto & underConstruction : m_bot.Buildings().getBuildings())
	{
		ss << underConstruction.type.getName() << "\n";
	}
	for (auto & incompletUpgrade : incompletUpgrades)
	{
		ss << incompletUpgrade.first.getName() << "\n";
	}
	m_bot.Map().drawTextScreen(0.01f, 0.4f, ss.str(), CCColor(255, 255, 0));	
}

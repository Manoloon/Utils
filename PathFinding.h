level class
struct Tile 
{
	Tile type;
	int columnIndex;
	int rowIndex;
	sf::Sprite sprite;
	int H;
	int G;
	int F;
	Tile* parentNode;
}

void ResetNodes()
{
	for(int i=0;i<GRID_WIDTH;++i)
	{
		for(int j=0;j<GRID_HEIGHT;++j)
		{
			m_grid[i][j].parentNode =nullptr;
			m_grid[i][j].H=0;
			m_grid[i][j].G=0;
			m_grid[i][j].F=0;
		}
	}
}

Enemy class 
private:
std::vector<sf::Vector2f> m_targetPositions;
sf::Vector2f m_currentTarget;
public:
void UpdatePathFinding(Level& level,sf::Vector2f playerPosition)
{
	//vars
	std::vector<Tile*> openList;
	std::vector<Tile*> closeList;
	std::vector<Tile*> pathList;
	std::vector<Tile*>::iterator position;
	Tile* currentNode;
	
	
	//reset node
	level.ResetNodes();
	m_targetPositions.clear();
	
	//store the start position node and the goal nodes
	Tile* startNode = level.GetTIle(m_position);
	Tile* GoalNode = level.GetTile(playerPosition);
	
	//Check we have a valid path to find. If not we just end function.
	if(startNode == goalNode)
	{
		m_targetPositions.clear();
		return;
	}
	// Pre-compute H cost (estimated cost to goal)
	for(int i=0;i<level.GetSize().x;++i)
	{
		for(int j=0;j<level.GetSize().y;++j)
		{
			int rowOffset,  heightOffset;
			Tile* node = level.GetTile(i,j);
			heightOffset = abs(node->rowIndex - goalNode->rowIndex);
			rowOffset = abs(node->columnIndex - goalNode->columnIndex);
			node->H = heightOffset + rowOffset;
		}
	}
	// Add the start node to the openList.
	openList.push_back(startNode);
	//while we have values to check in the openList.
	while(!openList.empty())
	{
		//Find node in the openList with the lowest F value and mark it as current.
		int lowestF = INT_MAX;
		for(Tile* tile : openList)
		{
			if(tile->F < lowestF)
			{
				lowestF = tile->F;
				currentNode = tile;
			}
		}
		//Remove current node from openList and add it to closeList
		position = std::find(openList.begin(),openList.end(),currentNode);
		if(position != openList.end())
		{
			openList.erase(position);
		}
		closeList.push_back(currentNode);
	}
	// Find all valid adjacent nodes.
	std::vector<Tile*> adjacentTiles;
	Tile* node;
	
	//NO DIAGONAL MOVEMENT ALLOW
	//start with the one in front.
	node = level.GetTile(currentNode->columnIndex,currentNode->rowIndex -1);
	if((node !=nullptr) && (level.IsFloor(*node)))
	{
		adjacentTiles.push_back(level.GetTile(currentNode->columnIndex, currentNode->rowIndex -1));
	}
	// Right node
		node = level.GetTile(currentNode->columnIndex+1,currentNode->rowIndex);
	if((node !=nullptr) && (level.IsFloor(*node)))
	{
		adjacentTiles.push_back(level.GetTile(currentNode->columnIndex+1, currentNode->rowIndex));
	}
	// Bottom node
		node = level.GetTile(currentNode->columnIndex,currentNode->rowIndex+1);
	if((node !=nullptr) && (level.IsFloor(*node)))
	{
		adjacentTiles.push_back(level.GetTile(currentNode->columnIndex, currentNode->rowIndex+1));
	}
	//Left node
		node = level.GetTile(currentNode->columnIndex-1,currentNode->rowIndex);
	if((node !=nullptr) && (level.IsFloor(*node)))
	{
		adjacentTiles.push_back(level.GetTile(currentNode->columnIndex-1, currentNode->rowIndex));
	}
	
	//For all adjacent nodes. We check if the node is the goal node ,because if it so,
	// that means that this is the best lowest F path.
	for(Tile* node : adjacentTiles)
	{
		if(node == goalNode)
		{
			node->parentNode = currentNode;
			//store the current path
			while(node->parentNode !=nullptr)
			{
			pathList.push_back(node);
			node = node->parentNode;
			}
			// Empty the openList and break the loop.
			openList.clear();
			break;
		}
		else
		{
			//if the node is not in the closeList
			position = std::find(closeList.begin(),closeList.end(),node);
			if(position == closeList.end())
			{
				//if the node is not in the openList
				position = std::find(openList.begin(),openList.end(),node);
				if(position == openList.end())
				{
					//add the node to the openList
					openList.push_back(node);
					//set the parent of the node to the currentNode
					node->parentNode = currentNode;
					
					//Calculate G (total movement cost so far) cost
					node->G = currentNode->G +10;
					//calculate the F ( total movement cost + Heuristic) cost
					node->F = node->G + node->H;
				}
				else
				{
					// check if this path is quicker than the other.
					int tempG = currentNode->G +10;
					if(tempG < node->G)
					{
						//re-parent node to this one
						node->parentNode = currentNode;
					}
				}
			}
			
		}
	}
	//Store the node locations as the enemy target locations.
	for(Tile* tile:pathList)
	{
		m_targetPositions.push_back(level.GetActualTileLocation(tile->columnIndex,tile->rowIndex));
	}
	//Reverse the target position as we read them from goal to origin.
	std::reverse(m_targetPositions.begin(),m_targetPositions.end());
}


// implementation of the algorithm on Enemy
Enemy.cpp
Update()
{
	if(!m_targetPositions.empty())
	{
		sf::Vector2f targetLocation = m_targetPositions.front();
		// Manhattan distance
		m_velocity = sf::Vector2f(targetLocation.x - m_position.x,targetLocation.y - m_position.y);
		if(abs(m_velocity.x) < 10.f && abs(m_velocity.y)<10.f)
		{
			m_targetPositions.erase(m_targetPositions.begin());
		}
		else
		{
			float length = sqrt(m_velocity.x * m_velocity.x+m_velocity.y * m_velocity.y);
			m_velocity.x /=length;
			m_velocity.y /=length;
			
			m_position.x += m_velocity.x * (m_speed * DeltaTime);
			m_position.y += m_velocity.y * (m_speed * DeltaTime);
			
			m_sprite.setPosition(m_position);
		}
	}
	// call character update
	Entity::Update(DeltaTime);
}

Game.h
Tile* m_playerPrevTile;

Game.cpp
game::update(deltaTime)
{
	//check if the player has moved grid square
	Tile* playerCurrentTile = m_level.GetTile(playerPosition);
	if(m_playerPrevTile != playerCurrentTile)
	{
		//store the new tile
		m_playerPrevTile = playerCurrentTile;
		//update path for all enemies
		for(const auto& enemy : m_enemies)
		{
			if(DistanceBetweenPoints(enemy->GetPosition(),playerPosition)<300.f)
			{
				enemy->UpdatePathFinding(m_level,playerPosition);
			}
		}
	}
}

// Debug del pathfinding
enemy.h
// override default draw function
void Draw(sf::RenderWindow& window,float timeDelta)override;

sf::Sprite m_pathSprite;
sf::Font m_debugFont;
sf::Text m_debugText;

Enemy()
{
int textureID = TextureManager::AddTexture("path.png");
m_pathSprite.setTexture(TextureManager::GetTexture(textureID);

sf::Vector2u spriteSize=m_pathSprite.getTexture()->GetSize();
m_pathSprite.setOrigin(sf::Vector2f(static_cast<float>(spriteSize.x/2),static_cast<float>(spriteSize.y/2));

m_Font.LoadFromFile("fsdas.TTF");
m_Text.setFont(m_Font);
m_Text.setCharacterSize(12);

void Draw(sf::RenderWindow& window,float timeDelta)
{
	//Debug the current path
	for(int =0;i<m_targetPositions.size();i++)
	{
		m_pathSprite.setPosition(m_targetPositions[i]);
		window.draw(m_pathSprite);
		
		std::ostringstream ss;
		ss << i;
		std::string str(ss.str());
		m_Text.setString(str);
		m_Text.setPosition(m_targetPositions[i]);
		window.draw(m_Text);
	}
}






























Util.h
//Tiles 
// Tiles.
enum class TILE {
	WALL_SINGLE,
	WALL_TOP_END,
	WALL_SIDE_RIGHT_END,
	WALL_BOTTOM_LEFT,
	WALL_BOTTOM_END,
	WALL_SIDE,
	WALL_TOP_LEFT,
	WALL_SIDE_LEFT_T,
	WALL_SIDE_LEFT_END,
	WALL_BOTTOM_RIGHT,
	WALL_TOP,
	WALL_BOTTOM_T,
	WALL_TOP_RIGHT,
	WALL_SIDE_RIGHT_T,
	WALL_TOP_T,
	WALL_INTERSECTION,
	WALL_DOOR_LOCKED,
	WALL_DOOR_UNLOCKED,
	WALL_ENTRANCE,
	FLOOR,
	FLOOR_ALT,
	EMPTY,
	COUNT
};
-1-
0x0 = 0001 = WALL_TOP_END 
-0-


// BackTracking Algorithm
level.h
/*Generate random map*/
void GenerateLevel()
{
	//create initial grid pattern
	for(int i=0;i<GRID_WIDTH;++i)
	{
	
		for(int j=0;j<GRID_HEIGTH;++j)
		{
			if((i%2 !=0) && (j%2 !=0))
			{
				// Tiles pares , nada .
				m_grid[i][j].type = TILE::EMPTY;
			}
			else
			{
				m_grid[i][j].type = TILE::WALL_TOP;
				m_grid[i][j].sprite.setTexture(TextureManager::GetTexture(m_textureIDs[static_cast<int>(TILE::WALL_TOP)]));
			}
			//set position
			m_grid[i][j].sprite.setPosition(m_origin.x+(TILE_SIZE*i),m_origin.y+(TILE_SIZE*j));
		}
	}
		// make the first carvePassage 
		CreatePath(1,1);
		CreateRooms(10);
		CalculateTexture();
}
void CreateRooms(int roomCount)
{
	for (int i=0;i<roomCount;++i)
	{
		// generate room size
		int roomWidth = std::rand()%2+1;
		int roomHeight = std::rand()%2+1;
		//choose random starting location
		int startX = std::rand() %(GRID_WIDTH-2)+1;
		int startY = std::rand()% (GRID_HEIGTH-2)+1;
		for(int x=-1;x<roomWidth;++x)
		{
			for(int z=-1;z<roomHeight;++z)
			{
				int newX = startX + x;
				int newY = startY + z;
				
				//check if the tile is valid
				if(TileIsValid(newX,newY)
				{
					//check the tile is not on an outer wall
					if((newX !=0) && (newX !=(GRID_WIDTH -1)) & (newY !=0) && (newY !=GRID_HEIGTH-1))
					{
						m_grid[newX][newY].type = TILE::FLOOR;
						m_grid[newX][newY].sprite.setTexture(TextureManager::GetTexture(m_textureIDs[static_cast<int>(TILE::FLOOR)]));
					}
				}
			}
		}
	}
}

void CreatePath(int columnIndex,int rowIndex)
{
	// store current tile
	Tile* currentTile = &m_grid[columnIndex][rowIndex];
	
	//create a list of possible directions and sort randomly
	sf::Vector2i directions[]={{0,-2},{2,0},{0,2},{-2,0}};
	std::random_shuffle(std::begin(directions),std::end(directions));
	
	//for each direction
	for (int i=0;i<4;++i)
	{
		//get the new tile position
		int dx = currentTile->columnIndex + direction[i].x;
		int dy = currentTile->rowIndex + direction[i].y;
		
		//if the tile is valid
		if(TileIsValid(dx,dy))
		{
			//store the tile
			Tile* tile = &m_grid[dx][dy];
			
			// if the tile has not been visited
			if(tile->type == TILE::EMPTY)
			{
				// mark the tile as floor
				tile->type = TILE::FLOOR;
				tile->sprite.setTexture(TextureManager::GetTexture(m_textureIDs[static_cast<int>(TILE::FLOOR)]));
				
				//knock that wall down
				int ddx = currentTile->columnIndex +(directions[i].x /2);
				int ddy = currentTile->rowIndex + (directions[i].y /2);
				
				Tile* wall = &m_grid[ddx][ddy];
				wall->type = TILE::FLOOR;
				wall->sprite.setTexture(TextureManager::GetTexture(m_textureIDs[static_cast<int>(TILE::FLOOR)]));
				
				//recursively call the function with the new tile
				CreatePath(dx,dy);
			}
		}
	}
}
void CalculateTexture()
{
	//calculate the correct texture for each tile in the level.
	for(int i=0;i<GRID_WIDTH;++i)
	{
		for(int j=0;j<GRID_HEIGHT;++j)
		{
			//check that the tile is a wall block
			if(IsWall(i,j))
			{
				//calculate bit mask
				int value = 0;
				//store the current type as default
				TILE type = m_grid[i][j].type;
				//TOP
				if(IsWall(i,j-1))
				{
					value +=1;
				}
				//RIGHT
				if(IsWall(i+1,j))
				{
					value +=2;
				}
				//BOTTOM
				if(IsWall(i,j+1))
				{
					value +=4
				}
				//LEFT
				if(IsWall(i-1,j))
				{
					value +=8;
				}
				//Set the new type
				m_grid[i][j].type = static_cast<TILE>(value);
				m_grid[i][j].sprite.setTexture(TextureManager::GetTexture(m_textureIDs[value]));
			}
		}
	}
}


game.cpp
in initialize()
{
	GenerateLevel();
}
// esta funcion tambien se crea en el game.h
void GenerateLevel()
{
	m_level.GenerateLevel();
	
	SpawnItem(ITEM::KEY);
	
	PopulateLevel();
}
	
#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <vector>
#include "DEFINITIONS.hpp"

namespace Sonar {
	class Pipe {
	public:
		Pipe(GameDataRef data);

		void SpawnBottomPipe();
		void SpawnTopPipe();
		void SpawnInvisiblePipe();
		void SpawnScoringPipes();
		void MovePipes(float dt);
		void DrawPipes();
		void RandomisePipeOffSet();

		const std::vector<sf::Sprite>& GetPipeSprites() const;

		std::vector<sf::Sprite>& GetScoringSprites();
	private:
		GameDataRef _data;
		std::vector<sf::Sprite> pipeSprites;
		std::vector<sf::Sprite> scoringPipes;
		int _landHeight;
		int _pipeSpawnYOffSet;
	};
}


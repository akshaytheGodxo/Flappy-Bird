#pragma once

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"
#include "Game.hpp"
namespace Sonar {
	class Flash {
	public:
		Flash(GameDataRef data);

		void Show(float dt);
		void Draw();

	private:
		GameDataRef _data;
		sf::RectangleShape _shape;
		bool _flashOn;
	};
}
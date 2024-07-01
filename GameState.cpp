#pragma once

#include "GameState.hpp"
#include <sstream>
#include "DEFINITIONS.hpp"
#include <iostream>
#include "Pipe.hpp"

namespace Sonar {
	GameState::GameState(GameDataRef data) : _data(data) {

	}

	void GameState::Init() {
		std::cout << "Game State" << std::endl;
		_data->assets.LoadTexture("Game Background", GAME_BACKGROUND_FILEPATH);
		_data->assets.LoadTexture("Pipe Up", PIPE_UP_FILEPATH);
		_data->assets.LoadTexture("Pipe Down", PIPE_DOWN_FILEPATH);

		pipe = new Pipe(_data);

		_background.setTexture(this->_data->assets.GetTexture("Game Background"));
	}

	void GameState::HandleInput() {
		sf::Event event;

		while (_data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type) {
				_data->window.close();
			}

			if (_data->input.IsSpriteClicked(_background, sf::Mouse::Left, _data->window)) {
				pipe->SpawnInvisiblePipe();
				pipe->SpawnBottomPipe();
				pipe->SpawnTopPipe();
			}

		}

	}

	void GameState::Update(float dt) {
		pipe->MovePipes( dt);
	}

	void GameState::Draw(float dt) {
		_data->window.clear();

		_data->window.draw(_background);

		pipe->DrawPipes();

		_data->window.display();
	}
}
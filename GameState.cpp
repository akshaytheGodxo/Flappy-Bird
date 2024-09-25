#pragma once

#include "GameState.hpp"
#include "DEFINITIONS.hpp"
#include "GameOverState.hpp"
#include <sstream>
#include <iostream>
#include <vector>


namespace Sonar {
	GameState::GameState(GameDataRef data) : _data(data) {

	}

	void GameState::Init() {
		if (!_hitSoundBuffer.loadFromFile(HIT_SOUND_FILEPATH)) {
			std::cout << "Error Loading Hit sound effect\n";
		}
		if (!_pointSoundBuffer.loadFromFile(POINT_SOUND_FILEPATH)) {
			std::cout << "Error Loading Point sound effect\n";
		}
		if (!_wingSoundBuffer.loadFromFile(WING_SOUND_FILEPATH)) {
			std::cout << "Error Loading Wing sound effect\n";
		}

		_hitSound.setBuffer(_hitSoundBuffer);
		_pointSound.setBuffer(_pointSoundBuffer);
		_wingSound.setBuffer(_wingSoundBuffer);

		_data->assets.LoadTexture("Game Background", GAME_BACKGROUND_FILEPATH);
		_data->assets.LoadTexture("Pipe Up", PIPE_UP_FILEPATH);
		_data->assets.LoadTexture("Pipe Down", PIPE_DOWN_FILEPATH);
		_data->assets.LoadTexture("Land", LAND_FILEPATH);
		_data->assets.LoadTexture("Bird Frame 1", BIRD_FRAME_1_FILEPATH);
		_data->assets.LoadTexture("Bird Frame 2", BIRD_FRAME_2_FILEPATH);
		_data->assets.LoadTexture("Bird Frame 3", BIRD_FRAME_3_FILEPATH);
		_data->assets.LoadTexture("Bird Frame 4", BIRD_FRAME_4_FILEPATH);
		_data->assets.LoadTexture("Scoring Pipe", SCORING_PIPE_FILEPATH);
		_data->assets.LoadFont("Flappy Font", FLAPPY_FONT_FILEPATH);



		pipe  = new Pipe(_data);
		land  = new Land(_data);
		bird  = new Bird(_data);
		flash = new Flash(_data);
		hud = new HUD(_data);


		_background.setTexture(this->_data->assets.GetTexture("Game Background"));
		_score = 0;
		hud->UpdateScore(_score);
		_gameState = GameStates::eReady;
	
	}

	void GameState::HandleInput() {
		sf::Event event;

		while (_data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type) {
				_data->window.close();
			}

			if (_data->input.IsSpriteClicked(_background, sf::Mouse::Left, _data->window)) {
				
				if (GameStates::eGameOver != _gameState)
				{
					_gameState = GameStates::ePlaying;
					bird->Tap();
					_wingSound.play();
				}
			}

		}

	}
	 
	void GameState::Update(float dt) {

		if (GameStates::eGameOver != _gameState) {
			bird->Animate(dt);
			land->MoveLand(dt);
		}

		if (GameStates::ePlaying == _gameState) {
			pipe->MovePipes(dt);

			if (clock.getElapsedTime().asSeconds() > PIPE_SPAWN_FREQUENCY) {

				pipe->RandomisePipeOffSet();

				pipe->SpawnInvisiblePipe();
				pipe->SpawnBottomPipe();
				pipe->SpawnTopPipe();
				pipe->SpawnScoringPipes();
				clock.restart();
			}

			bird->Update(dt);
		
			std::vector<sf::Sprite> landSprites = land->GetSprites();
			std::vector<sf::Sprite> pipeSprites = pipe->GetPipeSprites();
			for (int i = 0; i < landSprites.size(); i++) {
				if (collision.CheckSpriteCollision(bird->GetSprite(), landSprites.at(i))) {
					_gameState = GameStates::eGameOver;
					clock.restart();
					_hitSound.play();
				}
			}

			
			for (int i = 0; i < pipeSprites.size(); i++) {
				if (collision.CheckSpriteCollision(bird->GetSprite(), pipeSprites.at(i))) {
					_gameState = GameStates::eGameOver; 
					clock.restart();
					_hitSound.play();
				}
			}


			if (GameStates::ePlaying == _gameState) {
				std::vector<sf::Sprite>& scoringSprites = pipe->GetScoringSprites();
				for (int i = 0; i < scoringSprites.size(); i++) {
					if (collision.CheckSpriteCollision(bird->GetSprite(), scoringSprites.at(i))) {
						_score++;
						hud->UpdateScore(_score);

						scoringSprites.erase(scoringSprites.begin() + i);
					
						_pointSound.play();
					}
				}
			}
		}

		if (GameStates::eGameOver == _gameState) {
			flash->Show(dt);
			if (clock.getElapsedTime().asSeconds() > TIME_BEFORE_GAME_OVER_APPEARS) {
				_data->machine.AddState(StateRef(new  GameOverState(_data, _score)), true);
			}
		}
	}

	void GameState::Draw(float dt) {
		_data->window.clear();

		_data->window.draw(_background);

		pipe->DrawPipes();
		land->DrawLand();
		bird->Draw();
		flash->Draw();
		hud->Draw();
		_data->window.display();
	}
}
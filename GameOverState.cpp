#pragma once

#include "GameOverState.hpp"
#include "GameState.hpp"
#include <sstream>
#include "DEFINITIONS.hpp"
#include <iostream>
#include <fstream>

namespace Sonar {
	GameOverState::GameOverState(GameDataRef data, int score) : _data(data), _score(score) {

	}

	void GameOverState::Init() {
		std::ifstream readFile;
		readFile.open("Resources/High-Score.txt");

		if (readFile.is_open()) {
			while (!readFile.eof()) {
				readFile >> _highScore;
			}
		}

		readFile.close();

		std::ofstream writeFile("Resources/High-Score.txt");

		if (writeFile.is_open()) {
			if (_score > _highScore) {
				_highScore = _score;
			}
		
			writeFile << _highScore;
		}
		writeFile.close();

		_data->assets.LoadTexture("Game Over Background", GAME_OVER_BACKGROUND_FILEPATH);
		_data->assets.LoadTexture("Game Over Title", GAME_OVER_TITLE_FILEPATH);
		_data->assets.LoadTexture("Game Over Body", GAME_OVER_BODY_FILEPATH);
		_data->assets.LoadTexture("Bronze Medal", BRONZE_MEDAL_FILEPATH);
		_data->assets.LoadTexture("Silver  Medal", SILVER_MEDAL_FILEPATH);
		_data->assets.LoadTexture("Gold Medal", GOLD_MEDAL_FILEPATH);
		_data->assets.LoadTexture("Platinum Medal", PLATINUM_MEDAL_FILEPATH);


		_background.setTexture(this->_data->assets.GetTexture("Game Over Background"));
		_gameOverTitle.setTexture(this->_data->assets.GetTexture("Game Over Title"));
		_gameOverContainer.setTexture(this->_data->assets.GetTexture("Game Over Body"));
		_retryButton.setTexture(this->_data->assets.GetTexture("Play Button"));

		
		_gameOverContainer.setPosition(_data->window.getSize().x / 2 - _gameOverContainer.getGlobalBounds().width / 2, _data->window.getSize().y / 2 - _gameOverContainer.getGlobalBounds().height / 2);
		_gameOverTitle.setPosition(_data->window.getSize().x / 2 - _gameOverTitle.getGlobalBounds().width / 2, _gameOverContainer.getPosition().y - _gameOverTitle.getGlobalBounds().height * 1.2);

		_retryButton.setPosition(_data->window.getSize().x / 2 - _retryButton.getGlobalBounds().width / 2, _gameOverContainer.getPosition().y + _gameOverContainer.getGlobalBounds().height + _retryButton.getGlobalBounds().height * 0.2);

		_scoreText.setFont(_data->assets.GetFont("Flappy Font"));
		_scoreText.setString(std::to_string(_score));
		_scoreText.setCharacterSize(56);
		_scoreText.setFillColor(sf::Color::White);
		_scoreText.setOrigin(_scoreText.getGlobalBounds().width / 2, _scoreText.getGlobalBounds().height / 2);
		_scoreText.setPosition(_data->window.getSize().x / 10 * 7.25, _data->window.getSize().y / 2.15);


		_highScoreText.setFont(_data->assets.GetFont("Flappy Font"));
		_highScoreText.setString(std::to_string(_highScore));
		_highScoreText.setCharacterSize(56);
		_highScoreText.setFillColor(sf::Color::White);
		_highScoreText.setOrigin(_highScoreText.getGlobalBounds().width / 2, _highScoreText.getGlobalBounds().height / 2);
		_highScoreText.setPosition(_data->window.getSize().x / 10 * 7.25, _data->window.getSize().y / 1.78);
	
		if (_score >= PLATINUM_MEDAL_SCORE) {
			_medal.setTexture(_data->assets.GetTexture("Platinum Medal"));
		}
		else if (_score >= GOLD_MEDAL_SCORE) {
			_medal.setTexture(_data->assets.GetTexture("Gold Medal"));
		}
		else if (_score >= SILVER_MEDAL_SCORE) {
			_medal.setTexture(_data->assets.GetTexture("Silver Medal"));
		}
		else {
			_medal.setTexture(_data->assets.GetTexture("Bronze Medal"));
		}

		_medal.setPosition(175, 465);
	}

	void GameOverState::HandleInput() {
		sf::Event event;

		while (_data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type)
				_data->window.close();
		 
			
			if (_data->input.IsSpriteClicked(_retryButton, sf::Mouse::Left, _data->window)) {
				_data->machine.AddState(StateRef(new GameState(_data)), true);
			}
		}

	}

	void GameOverState::Update(float dt) {

	}

	void GameOverState::Draw(float dt) {
		_data->window.clear();

		_data->window.draw(_background);

		_data->window.draw(_gameOverContainer);
		_data->window.draw(_gameOverTitle);
		_data->window.draw(_retryButton);
		_data->window.draw(_scoreText);
		_data->window.draw(_highScoreText);
		_data->window.draw(_medal);
		_data->window.display();
	}
}
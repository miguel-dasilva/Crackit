#include "Game.hpp"
#include "Pair.hpp"
#include "GameController.hpp"
#include "GameUI.hpp"
#include "GameGrid.hpp"

#include <SDL.h>

/*
        
    Piece:
        - user can rotate the piece (90 degree left or 90 degree right)
        - pieces falls down (player can make the piece fall faster)

    Pair:
        - two pieces together
        
    PlacedPiece:
        - Doesnt move
        - Count how many of the same type are connected
    
    Level:
        - 8x16 grid
        - contain the edges

    Game
        - Contains all objects (Pieces, placed pieces, level)
        - Reset the game

*/
const Vec2D Game::DROP_VELOCITY = {0, DROP_SPEED};
const Vec2D Game::SPAWN_POINT = {3, 0};

void Game::SetupInputActions(GameController& controller)
{
	ButtonAction leftKeyAction;
		leftKeyAction.key = GameController::LeftKey();
		leftKeyAction.action = [this](uint32_t dt, InputState state)
			{
				if(mGameState == PLAYING_PAIR && mCurrentPair)
				{
					if(GameController::IsPressed(state))
					{
						Vec2D leftMostGridPos = mGameGrid.ScreenPosToGridPos(Vec2D(mCurrentPair->GetMostLeftPoint(), 0));
						leftMostGridPos = Vec2D(leftMostGridPos.GetX() - 1, leftMostGridPos.GetY());
						if (mGameGrid.IsCellAvailable(leftMostGridPos))
							mCurrentPair->MoveLeft(mGameGrid.GetCellSize());
					}
				}
			};

		controller.AddInputActionForKey(leftKeyAction);

		ButtonAction rightKeyAction;
		rightKeyAction.key = GameController::RightKey();
		rightKeyAction.action = [this](uint32_t dt, InputState state)
			{
				if(mGameState == PLAYING_PAIR && mCurrentPair)
				{
					if(GameController::IsPressed(state))
					{
						Vec2D rightMostGridPos = mGameGrid.ScreenPosToGridPos(Vec2D(mCurrentPair->GetMostRightPoint(), 0));
						rightMostGridPos = Vec2D(rightMostGridPos.GetX(), rightMostGridPos.GetY());
						if (mGameGrid.IsCellAvailable(rightMostGridPos))
							mCurrentPair->MoveRight(mGameGrid.GetCellSize());
					}
				}
			};

		controller.AddInputActionForKey(rightKeyAction);

		ButtonAction downKeyAction;
		downKeyAction.key = GameController::DownKey();
		downKeyAction.action = [this](uint32_t dt, InputState state)
			{
				if(mGameState == PLAYING_PAIR && mCurrentPair)
				{
					if(GameController::IsPressed(state))
					{
						mCurrentPair->SetVelocity(Vec2D(0, Game::DROP_SPEED_ENHANCED));
					}
					else {
						mCurrentPair->SetVelocity(Vec2D(0, Game::DROP_SPEED));
					}
				}
			};

		controller.AddInputActionForKey(downKeyAction);

		ButtonAction eKeyAction;
		eKeyAction.key = GameController::EKey();
		eKeyAction.action = [this](uint32_t dt, InputState state)
			{
				if(mGameState == PLAYING_PAIR && mCurrentPair)
				{
					if(GameController::IsPressed(state))
					{
						if (!mGameGrid.IsCellAvailable(mGameGrid.ScreenPosToGridPos(mCurrentPair->RotateRightResult().GetCenterPoint())))
							mCurrentPair->MoveLeft(mGameGrid.GetCellSize());
						mCurrentPair->RotateRight();
					}
				}
			};

		controller.AddInputActionForKey(eKeyAction);

		ButtonAction QKeyAction;
		QKeyAction.key = GameController::QKey();
		QKeyAction.action = [this](uint32_t dt, InputState state)
			{
				if(mGameState == PLAYING_PAIR && mCurrentPair)
				{
					if(GameController::IsPressed(state))
					{
						if (!mGameGrid.IsCellAvailable(mGameGrid.ScreenPosToGridPos(mCurrentPair->RotateLeftResult().GetCenterPoint())))
							mCurrentPair->MoveRight(mGameGrid.GetCellSize());
						mCurrentPair->RotateLeft(mGameGrid.GetCellSize());
					}
				}
			};

		controller.AddInputActionForKey(QKeyAction);

		ButtonAction KKeyAction;
		KKeyAction.key = GameController::KKey();
		KKeyAction.action = [this](uint32_t dt, InputState state)
			{
				if(GameController::IsPressed(state))
				{
					Reset();
				}
			};

		controller.AddInputActionForKey(KKeyAction);
}

void Game::HandleGameOver()
{
	//Stop the game logic and wait for user input
}

void Game::HandleSpawningPairState()
{
	if (!mCurrentPair) {

        mCurrentPair = std::make_unique<Pair>(SPAWN_POINT, mGameGrid.GetCellSize());

        mCurrentPair->SetVelocity(Vec2D(0, DROP_SPEED));

		ResetMultiplier();

        mGameState = PLAYING_PAIR;

    } 
}

void Game::HandlePlayingPairState(uint32_t dt)
{
	if(mCurrentPair) {
		Vec2D bottomGridPos = GetBottomGridPosition();
		
		if(IsPairMoveableDown(bottomGridPos)) {
			mCurrentPair->Update(dt);
		} else {
			if (bottomGridPos.GetY() <= GAME_OVER_ROW)
			{
				mGameState = GAME_OVER;
			} else {
				LockCurrentPair();
				mGameGrid.DropCells();
				mGameState = CHECKING_GROUPS;

			}
		}
    }
}

void Game::LockCurrentPair() {
	AddLockedPiece(mCurrentPair->GetFirstPiece());
	AddLockedPiece(mCurrentPair->GetSecondPiece());
	mCurrentPair.reset();
}

bool Game::IsPairMoveableDown(const Vec2D &bottomGridPos)
{
	return mGameGrid.IsCellAvailable(bottomGridPos) 
			&& !mGameGrid.IsPieceColliding(mCurrentPair->GetFirstPiece(), Vec2D(0, 1))
			&& !mGameGrid.IsPieceColliding(mCurrentPair->GetSecondPiece(), Vec2D(0, 1));
}

Vec2D Game::GetBottomGridPosition() {
	return mGameGrid.ScreenPosToGridPos(Vec2D(0, mCurrentPair->GetMostBottomPoint()));
}

void Game::UpdateScore() {
	mCurrentScore = mCurrentScore + SCORE_EARN_BASE * mMultiplier;
	mGameUI.SetCurrentScore(mCurrentScore);
}

void Game::HandleCheckingGroupsState()
{
	if (mGameGrid.CheckForGroups(mMultiplier)) {
		mGameGrid.DropCells();
	}
	else
	{
		UpdateScore();
		mGameState = SPAWNING_PAIR;
	}

}
void Game::Init(GameController &controller)
{
    mGameGrid = GameGrid();
	mGameUI = GameUI(Vec2D(mGameGrid.GetNumColls() * mGameGrid.GetCellSize(), 0));

	SetupInputActions(controller);

	Reset();
    
}
void Game::ResetGameAtLevel(int level) 
{
    mCurrentPair.reset();
    mGameGrid.Init();
    mGameUI.Init(COUNTDOWN_TIMER);
    mCurrentScore = 0;
    mMultiplier = 0;
    mCurrentLevel = level;
	mGameUI.SetLevel(mCurrentLevel);
    mGameUI.SetCurrentScore(mCurrentScore);
    mGameUI.SetGoalScore(mCurrentLevel * GOAL_MULTIPLIER);
    mGameState = SPAWNING_PAIR;
}
void Game::Reset()
{
	ResetGameAtLevel(1);
}

void Game::SetNextLevel()
{
	mCurrentLevel++;
	ResetGameAtLevel(mCurrentLevel);
}

void Game::Update(uint32_t dt)
{
	switch(mGameState)
	{
		case SPAWNING_PAIR:
			HandleSpawningPairState();
			break;
		case PLAYING_PAIR:
			HandlePlayingPairState(dt);
			break;
		case CHECKING_GROUPS:
			HandleCheckingGroupsState();
			break;
		case GAME_OVER:
			HandleGameOver();
			break;
	}

	if (mCurrentScore >= mGameUI.GetGoalScore()) {
		SetNextLevel();
	} else if (mGameUI.GetCurrentTime() <= 0) {
		mGameState = GAME_OVER;
	}
	mGameUI.Update(dt);
}

void Game::Draw(Screen& screen)
{
	if (mGameState != GAME_OVER) {
		mGameUI.Draw(screen, mGameGrid.GetNumColls() * mGameGrid.GetCellSize());
	}
	else
	{
		mGameUI.DrawGameOver(screen, mGameGrid.GetNumColls() * mGameGrid.GetCellSize());
	}

    mGameGrid.Draw(screen);
    if (mCurrentPair)
        mCurrentPair->Draw(screen);
}

void Game::AddLockedPiece(Piece piece)
{
    mGameGrid.AddLockedPiece(piece);
}



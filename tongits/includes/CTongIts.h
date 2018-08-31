#ifndef CTONGITS_H
#define CTONGITS_H

#include "../../font/font.h"

#include "CPlayer.h"
#include "CDeck.h"

#define		ROBOT1		0
#define		ROBOT2		1
#define		HUMAN		2

//modes
#define		INTRO		0
#define		INGAME		1
#define		INRULES		2
#define     INCREDITS   3
#define		INMENU		4

class CTongIts
{
public:
	CTongIts()	{}
	~CTongIts() {
		freeFont(font1);
		freeFont(font2);
	}

	void Initialize(SDL_Surface *pScreen);

	void NewGame();
	void DrawScene();
	void DrawStatusBar();
	void HandleEvents(SDL_Event &pEvent, int *done);

	void SetActivePlayer(int p1, int p2, int p3);
	void SetMano();
	void SwitchPlayer();
	void StartItsTurn();	//start the computer's turn, executes a series of moves
	void SetNumPlayers(int pNumPlayers);

	void WriteStatus(char *msg, ...);

	bool GameOver();
	void DetermineWinner();
	void HandleCall();
	int  DetermineCallWinner();
	void HumanReveal();

	void SetAggresive(bool pAggression)		{ mDeck.mAggressiveRobot = mAggressiveRobot = pAggression; }
	bool Aggressive()						{ return mAggressiveRobot; }

	void DrawIntro();
	int  HandleIntro();

	void DrawRules(int pPage);
	void HandleRules();
	
    void DrawCredits();
    void HandleCredits();

	int  HandleButtons(SDL_Event &event);
	
	void DrawMenu();
	int  HandleMenu();

private:
	CDeck			mDeck;
	SDL_Surface*	mScreen;
	SDL_Surface*	mStatusBar;
	SDL_Surface*	mIntro;
	SDL_Surface*    mCredits;
	SDL_Surface*	mRules[7];
	SDL_Surface*	mMenu;
	SDL_Surface*	mFader;
	CCardRegion*	mSourceRgn;
	bool			mStartDrag;
	bool			mGameOver;
	int				mNumGames;
	int				mNumPlayers;

	int				mMode;
	int				mWins;	//no. of wins by human
	int				mLastBunot;

	SDLFont *font1;        // 2 fonts
	SDLFont *font2;

	bool			mAggressiveRobot;		//if true, always 'calls' if it can, if false, never 'calls'

	CPlayer			mPlayer[3];

};
#endif //CTONGITS_H

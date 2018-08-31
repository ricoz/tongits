#include "../includes/CTongIts.h"
#include <stdio.h>

void CTongIts::Initialize(SDL_Surface *pScreen)  // must be called only once
{
	mScreen		= pScreen;

	mDeck.Init(pScreen);		//the deck, holds all the card groups; creates the regions
	mStartDrag	= false;
	mGameOver	= true;
	mNumGames	= 0;
	mNumPlayers	= 3;

	mMode		= INTRO;
	mWins		= 0;
	mLastBunot	= 0;
	
	//initialize players' mIdx
	for(int i =0; i < mNumPlayers; i++)
		mPlayer[i].SetIdx(i);				//human is the last player in the array

	//window caption
	SDL_WM_SetCaption("Tong-Its", NULL);

	//the fonts
	font1 = initFont("font/data/font1",0,0,0);
	font2 = initFont("font/data/font2",0,0,0);

	//the status bar
    mStatusBar = SDL_CreateRGBSurface(SDL_SWSURFACE, mScreen->w, 25/*height*/, mScreen->format->BitsPerPixel/*color depth*/, 0, 0, 0, 0);
	SDL_FillRect(mStatusBar, NULL, 0xFFFFFF);

	//the intro screen
	mIntro = SDL_LoadBMP("gfx/intro.bmp");

	//the credits screen
	mCredits = SDL_LoadBMP("gfx/credits.bmp");

	//the menu
	mMenu = SDL_LoadBMP("gfx/menu.bmp");
	SDL_SetColorKey(mMenu, SDL_SRCCOLORKEY, SDL_MapRGB(mMenu->format, 0, 0, 0));
	
	//the fader
	mFader = SDL_LoadBMP("gfx/fader.bmp");
	SDL_SetAlpha(mFader, SDL_SRCALPHA|SDL_RLEACCEL, 128);
	
	//the rules
	mRules[0] = SDL_LoadBMP("gfx/rules/1.bmp");
	mRules[1] = SDL_LoadBMP("gfx/rules/2.bmp");
	mRules[2] = SDL_LoadBMP("gfx/rules/3.bmp");
	mRules[3] = SDL_LoadBMP("gfx/rules/4.bmp");
	mRules[4] = SDL_LoadBMP("gfx/rules/5.bmp");
	mRules[5] = SDL_LoadBMP("gfx/rules/6.bmp");
	mRules[6] = SDL_LoadBMP("gfx/rules/7.bmp");
	
	//for the engine
	InitDeck(mScreen);
}
/*-----------------------------------------------------------------------------------------
I N T R O


-----------------------------------------------------------------------------------------*/
void CTongIts::DrawIntro()
{
	SDL_BlitSurface(mIntro, NULL, mScreen, NULL);
	SDL_Flip(mScreen);
}

bool PtInRect(int px, int py, SDL_Rect &pRect)
{
	if((pRect.x <= px && px <= pRect.w + pRect.x) && (pRect.y <= py && py <= pRect.h + pRect.y))
		return true;

	else
		return false;
}

int CTongIts::HandleIntro()
{
	if(mMode != INTRO)
		return 0;

	SDL_Rect StartNewGame;
	SDL_Rect HowTo;
	SDL_Rect Settings;
	SDL_Rect Credits;

	StartNewGame.w = HowTo.w = Settings.w = Credits.w = 75;
	StartNewGame.h = HowTo.h = Settings.h = Credits.h = 75;

	StartNewGame.y = HowTo.y = Settings.y = Credits.y = 365;

	StartNewGame.x = 266;
	HowTo.x        = 354;
	Settings.x     = 441;
//	Credits.x      = 528;
	Credits.x      = 441;

	SDL_Event event;
	int done = 0;

	this->DrawIntro();

	while(done == 0) {

//		this->DrawIntro();

		while(SDL_PollEvent(&event)) {

			switch(event.type) {

				case SDL_QUIT: return 1;
				
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) {

						return 1;	//quit the game
					}

				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT) {

						if(PtInRect(event.button.x, event.button.y, StartNewGame)) {

							mMode = INGAME;
							this->NewGame();
							this->DrawScene();
							done = 1;	//terminate the loop
						} else

						if(PtInRect(event.button.x, event.button.y, HowTo)) {

							this->HandleRules();
							this->DrawIntro();
						} else
						
/*						if(PtInRect(event.button.x, event.button.y, Settings)) {
						
							printf("Settings\n");
						} else
*/
						if(PtInRect(event.button.x, event.button.y, Credits)) {

							this->HandleCredits();
							this->DrawIntro();
						}

					}
					break;
			}//end of switch
		}

		SDL_Delay(SDL_TIMESLICE);
	}

	return 0;
}

/*-----------------------------------------------------------------------------------------
R U L E S
C R E D I T S
M E N U
-----------------------------------------------------------------------------------------*/
void CTongIts::DrawRules(int pPage)
{
	SDL_BlitSurface(mRules[pPage - 1], NULL, mScreen, NULL);
	SDL_Flip(mScreen);
}

void CTongIts::HandleRules()
{
	SDL_Event event;
	int done = 0;
	int page = 1;

	this->DrawRules(page);

	while(done == 0) {
	
//		this->DrawRules(page);

		while(SDL_PollEvent(&event)) {
		
			switch(event.type) {

				case SDL_QUIT:
     				done = 1;

					break;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) {

						done = 1;
					}

					break;

				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT) {

						page--;
						if(page < 1) page = 7;
						
						this->DrawRules(page);
					} else
					
					if(event.button.button == SDL_BUTTON_RIGHT) {

						page++;
						if(page > 7) page = 1;
						
						this->DrawRules(page);
					}

					break;

			}//end of switch
		}
		
		SDL_Delay(SDL_TIMESLICE);
	}
}

void CTongIts::DrawCredits()
{
    SDL_BlitSurface(mCredits, NULL, mScreen, NULL);
	SDL_Flip(mScreen);
}

void CTongIts::HandleCredits()
{
	SDL_Event event;
	int done = 0;
	int page = 1;

	this->DrawCredits();

	while(done == 0) {

//		this->DrawCredits();

		while(SDL_PollEvent(&event)) {

			switch(event.type) {

				case SDL_QUIT:
     				done = 1;
					break;

				case SDL_KEYDOWN:
				case SDL_MOUSEBUTTONDOWN:
					done = 1;

					break;

			}//end of switch
		}

		SDL_Delay(SDL_TIMESLICE);
	}
}

void CTongIts::DrawMenu()
{
	int menu_width = mMenu->w;
	int menu_height = mMenu->h;
	int window_width = mScreen->w;
	int window_height = mScreen->h;
	
	//draw the menu at the center of the game screen
	SDL_Rect to;
	to.x = (window_width / 2) - (menu_width / 2);
	to.y = (window_height / 2) - (menu_height / 2);

	//blit the fader
	SDL_BlitSurface(mFader, NULL, mScreen, NULL);
	
	//blit the menu
	SDL_BlitSurface(mMenu, NULL, mScreen, &to);

	//flip the screen
	SDL_Flip(mScreen);
}

int CTongIts::HandleMenu()
{
	SDL_Rect New;
	SDL_Rect HowTo;
	SDL_Rect Credits;
	SDL_Rect Exit;
	SDL_Rect Return;
	
	New.w = HowTo.w = Credits.w = Exit.w = Return.w = 141;
	New.h = HowTo.h = Credits.h = Exit.h = Return.h = 26;

	New.x = HowTo.x = Credits.x = Exit.x = Return.x = 249;

	New.y		= 170;
	HowTo.y		= 199;
	Credits.y	= 227;
	Exit.y		= 256;
	Return.y	= 286;


	SDL_Event event;
	int done = 0;
	int page = 1;

	SDL_Surface *background = SDL_DisplayFormat(mScreen);	//create a background
	SDL_SetClipRect(mScreen, NULL);	//remove clipping so that we can draw to the entire screen
	this->DrawMenu();

	while(done == 0) {
	
		while(SDL_PollEvent(&event)) {
		
			switch(event.type) {
			
				case SDL_QUIT:
     				return 1;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) {

						mMode = INGAME;
						done = 1;
					}

					break;

				case SDL_MOUSEBUTTONDOWN:
				    //NewGame
					if(PtInRect(event.button.x, event.button.y, New)) {

						mMode = INGAME;
						this->NewGame();
						this->DrawScene();
						done = 1;
					} else

					//How to Play
					if(PtInRect(event.button.x, event.button.y, HowTo)) {
						this->HandleRules();
						//re-display the menu
						SDL_BlitSurface(background, NULL, mScreen, NULL);
						this->DrawMenu();

					} else

					//Credits
					if(PtInRect(event.button.x, event.button.y, Credits)) {

						this->HandleCredits();
						//re-display the menu
						SDL_BlitSurface(background, NULL, mScreen, NULL);
      					this->DrawMenu();
					} else

					//Exit
					if(PtInRect(event.button.x, event.button.y, Exit)) {

						return 1;
					} else

					//Return to game
					if(PtInRect(event.button.x, event.button.y, Return)) {

						mMode = INGAME;
						done = 1;
					}

					break;


			}//end of switch
		}
		
		SDL_Delay(SDL_TIMESLICE);
	}

	this->DrawScene();
	return 0;
}

/*-----------------------------------------------------------------------------------------*/

void CTongIts::NewGame()
{
	//fix new game while dragging problem:
	if(mStartDrag)
		return;

	if(!mGameOver) {	//Starts a new game while a game is still active

		mPlayer[HUMAN].SetWon(true);
		for(int i = 0; i < mNumPlayers - 1; i++)
			mPlayer[i].SetWon(false);
	}
	mGameOver = false;

	for(int i = 0; i < mNumPlayers; i++)
		mPlayer[i].SetCanCall(false);

	//Empty the card regions from the previous game
	mDeck.mGame.EmptyStacks();

 	//Create then shuffle the Deck
	mDeck.mGame[CRD_NEWCARDPILE].NewDeck();
	mDeck.mGame[CRD_NEWCARDPILE].Shuffle();

	//Deal
	mDeck.mGame[CRD_ROBOT1].Push(mDeck.mGame[CRD_NEWCARDPILE].Pop(12));
	mDeck.mGame[CRD_ROBOT2].Push(mDeck.mGame[CRD_NEWCARDPILE].Pop(12));
	mDeck.mGame[CRD_HUMANPLAYER].Push(mDeck.mGame[CRD_NEWCARDPILE].Pop(12));

	mDeck.ClearSets();

	SetMano();

    //Initialize all card coordinates
	mDeck.mGame.InitAllCoords();

	StartItsTurn();
}

void CTongIts::SetActivePlayer(int p1, int p2, int p3)
{
	mPlayer[ROBOT1].SetTurn(p1);
	mPlayer[ROBOT2].SetTurn(p2);
	mPlayer[HUMAN].SetTurn(p3);
}

void CTongIts::DrawScene()
{
	DrawStatusBar();
	mDeck.mGame.DrawStaticScene();
}

void CTongIts::SetMano()
{
	SetActivePlayer(0, 0, 0);

	if(mNumGames == 0) {

		mPlayer[HUMAN].SetTurn(true);
		mPlayer[HUMAN].SetMano();
		mDeck.mGame[CRD_HUMANPLAYER].Push(mDeck.mGame[CRD_NEWCARDPILE].Pop());
		WriteStatus("Your Turn. You can either Sapaw, Reveal a set or Dump a card. Cards remaining: %i", mDeck.mGame[CRD_NEWCARDPILE].Size());
	}

	else {

		if(mPlayer[HUMAN].Won()) {

			mPlayer[HUMAN].SetTurn(true);
			mPlayer[HUMAN].SetMano();
			mDeck.mGame[CRD_HUMANPLAYER].Push(mDeck.mGame[CRD_NEWCARDPILE].Pop());
			WriteStatus("Your Turn. You can either Sapaw, Reveal a set or Dump a card. Cards remaining: %i", mDeck.mGame[CRD_NEWCARDPILE].Size());
		}

		else {

			for(int i = 0; i < mNumPlayers - 1; i++) {

				if(mPlayer[i].Won()) {

					mPlayer[i].SetTurn(true);
					mPlayer[i].SetMano();
					WriteStatus("Robot %i's Turn", i + 1);
					mDeck.mGame[i + 2].Push(mDeck.mGame[0].Pop());
					break;
				}
			}
		}
	}
}

void CTongIts::SetNumPlayers(int pNumPlayers)
{
	if(pNumPlayers < 2) pNumPlayers = 2;
	if(pNumPlayers > 4) pNumPlayers = 4;
	mNumPlayers = pNumPlayers;
}

bool CTongIts::GameOver()
{
	if(mGameOver = mDeck.mGame[CRD_NEWCARDPILE].Empty())
		return mGameOver;

	if(mGameOver = mDeck.HandIsEmpty(mPlayer[ROBOT1].GetIdx()))
		return mGameOver;

	if(mGameOver = mDeck.HandIsEmpty(mPlayer[ROBOT2].GetIdx()))
		return mGameOver;

	if(mGameOver = mDeck.HandIsEmpty(mPlayer[HUMAN].GetIdx()))
		return mGameOver;

	return false;
}

void CTongIts::DetermineWinner()
{
	if(!mGameOver || mDeck.ACallWasMade())
		return;

	for(int i =0; i < mNumPlayers; i++) {

		mPlayer[i].SetCanCall(false);
		mPlayer[i].SetHasSets(false);
		mPlayer[i].SetWon(false);
	}
	
	for(int i = 2; i < 5; i++) {

		mDeck.mGame[i].SetCardsFaceUp(true);
		mDeck.mGame[i].InitCardCoords();
	}

	mDeck.mGame.DrawStaticScene();

	mNumGames++;	//increment no. of games played

	if(mDeck.HandIsEmpty(ROBOT1)) {

		this->WriteStatus("Robot 1 wins! Tong its! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
		mPlayer[ROBOT1].SetWon(true);
	} else

	if(mDeck.HandIsEmpty(ROBOT2)) {

		this->WriteStatus("Robot 2 wins! Tong its! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
		mPlayer[ROBOT2].SetWon(true);
	} else
	
	if(mDeck.HandIsEmpty(HUMAN)) {

		mWins++;
		this->WriteStatus("You win! Tong its! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
		mPlayer[HUMAN].SetWon(true);
		AnimateCards();
	}

	else {

		int Idx = mDeck.GetLowestHand(mLastBunot);

		if(Idx == CRD_ROBOT1) {

			this->WriteStatus("Robot 1 wins! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
			mPlayer[ROBOT1].SetWon(true);
		} else
		
		if(Idx == CRD_ROBOT2) {

			this->WriteStatus("Robot 2 wins! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
			mPlayer[ROBOT2].SetWon(true);
		} else
		
		if(Idx == CRD_HUMANPLAYER) {

			mWins++;
			this->WriteStatus("You win! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
			mPlayer[HUMAN].SetWon(true);
			AnimateCards();
		}
	}

//	mDeck.mGame.DrawStaticScene();
}

void CTongIts::SwitchPlayer()
{
	if(mDeck.ACallWasMade()) {

		this->HandleCall();
		return;
	}

	if(this->GameOver()) {
	
		if(mPlayer[HUMAN].IsItMyTurn())			mLastBunot = 2;
		else if(mPlayer[ROBOT2].IsItMyTurn())	mLastBunot = 1;
		else if(mPlayer[ROBOT1].IsItMyTurn())	mLastBunot = 0;

		this->DetermineWinner();
		return;
	}

	if(mPlayer[HUMAN].IsItMyTurn()) {

		SetActivePlayer(1, 0, 0);
		WriteStatus("Robot 1's Turn");
	} else
	
	if(mPlayer[ROBOT1].IsItMyTurn()) {

		SetActivePlayer(0, 1, 0);
		WriteStatus("Robot 2's Turn");
	} else
	
	if(mPlayer[ROBOT2].IsItMyTurn()) {

		SetActivePlayer(0, 0, 1);
		WriteStatus("Your Turn. You can either Chow or Get a new card. Cards remaining: %i", mDeck.mGame[CRD_NEWCARDPILE].Size());
	}

	else {

		SetActivePlayer(0, 0, 1);
		WriteStatus("Your Turn. You can either Chow or Get a new card. Cards remaining: %i", mDeck.mGame[CRD_NEWCARDPILE].Size());
	}
}

void CTongIts::StartItsTurn()
{
	if(mMode != INGAME)
		return;

	if(mGameOver || mDeck.ACallWasMade())
		return;

	if(mPlayer[HUMAN].IsItMyTurn())
		return;

	if(mPlayer[ROBOT1].IsItMyTurn()) {

		mPlayer[ROBOT1].StartTurn(mDeck);
		SwitchPlayer();
		StartItsTurn();
	} else

	if(mPlayer[ROBOT2].IsItMyTurn()) {

		mPlayer[ROBOT2].StartTurn(mDeck);
		SwitchPlayer();
		StartItsTurn();
	}

	return;
}

void CTongIts::HandleEvents(SDL_Event &pEvent,int *done)	//Only for human player, computer player acts on its own
{
	if(mMode != INGAME) return;	//not inside a game

	//New game
	if(pEvent.type == SDL_KEYDOWN && pEvent.key.keysym.sym == SDLK_n) {

		if(mStartDrag)
			return;

		this->NewGame();
		this->DrawScene();
	}

	if(mGameOver) {

		if(pEvent.type == SDL_MOUSEBUTTONDOWN && pEvent.button.button == SDL_BUTTON_LEFT) {
		
			if(mStartDrag)
				return;
		
			this->NewGame();
			this->DrawScene();
		}
	}
	
	if(!mPlayer[HUMAN].IsItMyTurn()) { //not human's turn, don't bother
	
		return;
	}

	switch(pEvent.type) {

		case SDL_KEYDOWN:
			//show the menu
			if(pEvent.key.keysym.sym ==SDLK_ESCAPE) {

   				mMode = INMENU;
       			*done = this->HandleMenu();
       		}
/*
			if(pEvent.key.keysym.sym == SDLK_SPACE) {	//Auto-play

				mPlayer[HUMAN].StartTurn(mDeck);
				SwitchPlayer();
				StartItsTurn();
			}
*/
			//Call
			if(pEvent.key.keysym.sym == SDLK_c) {

				if(mPlayer[HUMAN].CanCall() && !mDeck.Nasapawan[mPlayer[HUMAN].GetIdx()]) {

					mPlayer[HUMAN].Call(mDeck);
					SwitchPlayer();
					return;
				}
			}

			break;

		case SDL_MOUSEBUTTONDOWN:
			if(pEvent.button.button == SDL_BUTTON_LEFT) {	

			    *done = this->HandleButtons(pEvent);

				mSourceRgn = mDeck.mGame.OnMouseDown(pEvent.button.x, pEvent.button.y);
				if(mSourceRgn == NULL) break;

				if( (mSourceRgn->Id == CRD_HUMAN || mSourceRgn->Id == CRD_TEMP) && mDeck.mGame.InitDrag(pEvent.button.x, pEvent.button.y)) {

					mStartDrag = true;
					SDL_WM_GrabInput(SDL_GRAB_ON);
				}

/*-----------------------------------------------------------------------------------------*/
/*
				//clicked on the top of other opponents, dumps a card
				if(mSourceRgn->Id == CRD_ROBOT && (mPlayer[HUMAN].DidChow() || mPlayer[HUMAN].DidGetNewCard())) {

					if(mPlayer[HUMAN].Dump(mDeck)) {

						SwitchPlayer();
						StartItsTurn();
					}
				}
*/
				//clicked on the top of the valid sets, show valid sets, sapaw
				if( mSourceRgn->Id == CRD_SET && (mPlayer[HUMAN].DidChow() || mPlayer[HUMAN].DidGetNewCard()) ) {

//					if(mDeck.FindValidSet(mPlayer[HUMAN].GetIdx()))		//automatically searches for a valid set, not recommended for normal (natural) play
//						mPlayer[HUMAN].Show(mDeck, mSourceRgn->Idx);
					if(mPlayer[HUMAN].Show(mDeck, mSourceRgn->Idx))
     					;
					else
						mPlayer[HUMAN].Sapaw(mDeck, mSourceRgn->Idx);

					if(mDeck.HandIsEmpty(mPlayer[HUMAN].GetIdx())) {

						mGameOver = true;
						mLastBunot = 2;
						this->DetermineWinner();
						return;
					}
				}

				//clicked on the top of the new card pile, get a new card
				if(mSourceRgn->Id == CRD_NEW && (!mPlayer[HUMAN].DidChow() && !mPlayer[HUMAN].DidGetNewCard())) {

					mPlayer[HUMAN].GetNewCard(mDeck);
					WriteStatus("Your Turn. You can either Sapaw, Reveal a set or Dump a card. Cards remaining: %i", mDeck.mGame[CRD_NEWCARDPILE].Size());
				}

				//clicked on the top of the dump pile, perform chow
				if(mSourceRgn->Id == CRD_DUMP && (!mPlayer[HUMAN].DidChow() && !mPlayer[HUMAN].DidGetNewCard())) {

//					if(mDeck.CanChow(mPlayer[HUMAN].GetIdx()))
						mPlayer[HUMAN].Chow(mDeck);
						WriteStatus("Your Turn. You can either Sapaw, Reveal a set or Dump a card. Cards remaining: %i", mDeck.mGame[CRD_NEWCARDPILE].Size());

					if(mDeck.HandIsEmpty(mPlayer[HUMAN].GetIdx())) {

						mGameOver = true;
						mLastBunot = 2;
						this->DetermineWinner();
						return;
					}
  				}
/*-----------------------------------------------------------------------------------------*/
			}

			//substitute right-click for double-click event
			if(pEvent.button.button == SDL_BUTTON_RIGHT) {
			
				if(!mStartDrag) {

					mSourceRgn = mDeck.mGame.OnMouseDown(pEvent.button.x, pEvent.button.y);
					if(mSourceRgn == NULL) break;
				}

				if(mSourceRgn->Id == CRD_HUMAN)
					mDeck.mGame.SelectCard(pEvent.button.x, pEvent.button.y);

 				if(mSourceRgn->Id == CRD_TEMP && (mPlayer[HUMAN].DidChow() || mPlayer[HUMAN].DidGetNewCard())) {

					if(mDeck.ShowTemp(mSourceRgn->Idx))
						mPlayer[HUMAN].SetHasSets(true);
				}
				//revealing of dumped cards
				if(mSourceRgn->Id == CRD_DUMP) {

					mDeck.RevealDump();
				}
			}
			break;

		case SDL_MOUSEMOTION:
			if(pEvent.motion.state == SDL_BUTTON(1) && mStartDrag)
				mDeck.mGame.DoDrag(pEvent.motion.x, pEvent.motion.y);

			break;

		case SDL_MOUSEBUTTONUP:
			if(mStartDrag) {
			
				mStartDrag = false;
				mDeck.mGame.DoDrop();
				SDL_WM_GrabInput(SDL_GRAB_OFF);
			}

			break;
	}//end of switch
}

void CTongIts::DrawStatusBar()
{
	SDL_SetClipRect(mScreen, NULL);

	SDL_Rect clip;
	clip.x = 0;
	clip.y = 0;
	clip.w = mScreen->w;
	clip.h = mScreen->h - 20;

	SDL_Rect dest;
	dest.x = 0;
	dest.y = clip.h;
	dest.w = clip.w;
	dest.h = 20;

	SDL_BlitSurface(mStatusBar, NULL, mScreen, &dest);
	SDL_Flip(mScreen);

	SDL_SetClipRect(mScreen, &clip);
}

#include <stdarg.h>
void CTongIts::WriteStatus(char *msg, ...)
{
	char string[1024];                  // Temporary string

	va_list ap;                         // Pointer To List Of Arguments
	va_start(ap, msg);                  // Parses The String For Variables
	vsprintf(string, msg, ap);          // And Converts Symbols To Actual Numbers
	va_end(ap);

	SDL_FillRect(mStatusBar, NULL, 0xFFFFFF);
	drawString(mStatusBar, font2, 10, 2, string);
	DrawStatusBar();
}
/*-----------------------------------------------------------------------------------------
C A L L   F U N C T I O N S


-----------------------------------------------------------------------------------------*/

void CTongIts::HumanReveal()
{
	SDL_Event event;
	int done = 0;

	while(done == 0) {

		while(SDL_PollEvent(&event)) {

			switch(event.type) {

				case SDL_KEYDOWN:
					if(!mStartDrag)
	 					if(event.key.keysym.sym == SDLK_RETURN) { done = 1; }
	     			break;

				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT) {

						mSourceRgn = mDeck.mGame.OnMouseDown(event.button.x, event.button.y);
						if(mSourceRgn == NULL) break;

						if( (mSourceRgn->Id == CRD_HUMAN || mSourceRgn->Id == CRD_TEMP) && mDeck.mGame.InitDrag(event.button.x, event.button.y)) {

							mStartDrag = true;
							SDL_WM_GrabInput(SDL_GRAB_ON);
						}
					}

					if(event.button.button == SDL_BUTTON_RIGHT) {

						if(!mStartDrag) {

							mSourceRgn = mDeck.mGame.OnMouseDown(event.button.x, event.button.y);
							if(mSourceRgn == NULL) break;
						}

						if(mSourceRgn->Id == CRD_HUMAN)
							mDeck.mGame.SelectCard(event.button.x, event.button.y);

		 				if(mSourceRgn->Id == CRD_TEMP) {

							if(mDeck.ShowTemp(mSourceRgn->Idx))
								mPlayer[HUMAN].SetHasSets(true);
						}
					}
					break;

					//clicked on the top of the valid sets, show valid sets
					if(mSourceRgn->Id == CRD_SET) {

						mPlayer[HUMAN].Show(mDeck, mSourceRgn->Idx);
					}

				case SDL_MOUSEMOTION:
					if(event.motion.state == SDL_BUTTON(1) && mStartDrag)
						mDeck.mGame.DoDrag(event.motion.x, event.motion.y);

					break;

				case SDL_MOUSEBUTTONUP:
					if(mStartDrag) {

						mStartDrag = false;
						mDeck.mGame.DoDrop();
						SDL_WM_GrabInput(SDL_GRAB_OFF);
					}

					break;
			}
		}
		
		SDL_Delay(SDL_TIMESLICE);
	}
}

int  CTongIts::DetermineCallWinner()
{
	int loval = 1000;
	int Idx = 2;
	int val = 0;

	mNumGames++;	//increment no. of games played

	for(int i =0; i < mNumPlayers; i++) {

		mPlayer[i].SetWon(false);
	}

	for(int i = 2; i < 5; i++) {

		mDeck.mGame[i].SetCardsFaceUp(true);
		mDeck.mGame[i].InitCardCoords();
	}

	mDeck.mGame.DrawStaticScene();

	for(int j = 2; j < 5; j++) {

		if(j == CRD_HUMANPLAYER && mPlayer[j - 2].HasSets())
			val = mDeck.mGame[CRD_HUMANPLAYER].GetTotalRankValue() + mDeck.GetTempRankValue();

		else if(mPlayer[j - 2].HasSets())
			val = mDeck.mGame[j].GetTotalRankValue();

		else
  			val = 1000;

		if(val < loval) {

			loval = val;
			Idx = j;
		}
	}

	if(Idx == CRD_ROBOT1) {

		this->WriteStatus("Robot 1 wins! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
		mPlayer[ROBOT1].SetWon(true);
	} else

	if(Idx == CRD_ROBOT2) {

		this->WriteStatus("Robot 2 wins! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
		mPlayer[ROBOT2].SetWon(true);
	} else
	
	if(Idx == CRD_HUMANPLAYER) {

		mWins++;
		this->WriteStatus("You win! Click to start a new game. Total games played = %i, won %i game(s)", mNumGames, mWins);
		mPlayer[HUMAN].SetWon(true);
		AnimateCards();
	}

	for(int i =0; i < mNumPlayers; i++) {

		mPlayer[i].SetCanCall(false);
		mPlayer[i].SetHasSets(false);
	}

	return Idx;
}

void CTongIts::HandleCall() {

	SDL_ShowCursor(1);
	mGameOver		= true;
	bool success	= false;

	//Human Called
	if(mPlayer[HUMAN].IsItMyTurn()) {

		WriteStatus("You Called...");
		SDL_Delay(1000);

		if(mPlayer[ROBOT1].HasSets()) {

			WriteStatus("You Called... Robot 1 is revealing his sets...");
			do {

				if(mDeck.FindValidSet(mPlayer[ROBOT1].GetIdx()))
					success = mPlayer[ROBOT1].Show(mDeck, -1);
			} while(success);
		}

		if(mPlayer[ROBOT2].HasSets()) {

			WriteStatus("You Called... Robot 2 is revealing his sets...");

			do {

				if(mDeck.FindValidSet(mPlayer[ROBOT2].GetIdx()))
					success = mPlayer[ROBOT2].Show(mDeck, -1);
			} while(success);
		}

		this->DetermineCallWinner();
	} else

	//Robot 1 Called
	if(mPlayer[ROBOT1].IsItMyTurn()) {

		WriteStatus("Robot 1 Called...");
		SDL_Delay(1000);

		if(mPlayer[ROBOT2].HasSets()) {

			WriteStatus("Robot 1 Called... Robot 2 is revealing his sets...");
			do {

				if(mDeck.FindValidSet(mPlayer[ROBOT2].GetIdx()))
					success = mPlayer[ROBOT2].Show(mDeck, -1);
			} while(success);
		}

		if(mPlayer[HUMAN].HasSets()) {

			WriteStatus("Robot 1 Called... Please reveal your sets then press [Enter]...");
			HumanReveal();
		}

		this->DetermineCallWinner();
	} else
	
	//Robot 2 Called
	if(mPlayer[ROBOT2].IsItMyTurn()) {

		WriteStatus("Robot 2 Called...");
		SDL_Delay(1000);

		if(mPlayer[HUMAN].HasSets()) {

			WriteStatus("Robot 2 Called... Please reveal your sets then press [Enter]...");
			HumanReveal();
		}

		if(mPlayer[ROBOT1].HasSets()) {

			WriteStatus("Robot 2 Called... Robot 1 is revealing his sets...");

			do {

				if(mDeck.FindValidSet(mPlayer[ROBOT2].GetIdx()))
					success = mPlayer[ROBOT2].Show(mDeck, -1);
			} while(success);
		}

		this->DetermineCallWinner();
	}

	mDeck.mGame.DrawStaticScene();
}

int CTongIts::HandleButtons(SDL_Event &event)
{
	SDL_Rect Dump;
	SDL_Rect Show;
	SDL_Rect Chow;
	SDL_Rect Call;
	SDL_Rect Menu;
	
	Dump.w = Show.w = Chow.w = Call.w = Menu.w = 101;
	Dump.h = Show.h = Chow.h = Call.h = Menu.h = 26;

	Dump.x = Show.x = Chow.x = Call.x = Menu.x = 525;

	Dump.y = 299;
	Show.y = 327;
	Chow.y = 355;
	Call.y = 383;
	Menu.y = 411;

    //Dump
	if(PtInRect(event.button.x, event.button.y, Dump) && (mPlayer[HUMAN].DidChow() || mPlayer[HUMAN].DidGetNewCard())) {

		if(mPlayer[HUMAN].Dump(mDeck)) {

			mDeck.ResetDump();
			SwitchPlayer();
			StartItsTurn();
		}
	} else

	//Show
	if(PtInRect(event.button.x, event.button.y, Show) && (mPlayer[HUMAN].DidChow() || mPlayer[HUMAN].DidGetNewCard())) {

        mPlayer[HUMAN].Show(mDeck, -1);

		if(mDeck.HandIsEmpty(mPlayer[HUMAN].GetIdx())) {

			mGameOver = true;
			mLastBunot = 2;
			this->DetermineWinner();

			return 0;
		}
	} else
	
    //Chow
	if(PtInRect(event.button.x, event.button.y, Chow) && (!mPlayer[HUMAN].DidChow() && !mPlayer[HUMAN].DidGetNewCard())) {

		mPlayer[HUMAN].Chow(mDeck);

		if(mDeck.HandIsEmpty(mPlayer[HUMAN].GetIdx())) {

			mGameOver = true;
			mLastBunot = 2;
			this->DetermineWinner();

			return 0;
		}
	} else
	
    //Call
	if(PtInRect(event.button.x, event.button.y, Call)) {

		if(mPlayer[HUMAN].CanCall() && !mDeck.Nasapawan[mPlayer[HUMAN].GetIdx()]) {

			mPlayer[HUMAN].Call(mDeck);
			SwitchPlayer();

			return 0;
		}
	} else
	
    //Show the menu
	if(PtInRect(event.button.x, event.button.y, Menu)) {

		mMode = INMENU;
        return this->HandleMenu();
	}
	
	return 0;
}

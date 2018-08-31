#include "../includes/CPlayer.h"
#include <stdio.h>

bool CPlayer::GetNewCard(CDeck &pDeck)
{
	if(mDidGetNewCard || mDidChow)
		return false;

	if(!pDeck.GetNewCard(mIdx))
		return false;

	mCanCall = false;
	mDidGetNewCard = true;

	return mDidGetNewCard;
}

bool CPlayer::Chow(CDeck &pDeck)
{
	if(mDidGetNewCard || mDidChow)
		return false;

	if(!pDeck.Chow(mIdx))
		return false;

	mHasSets = true;
	mCanCall = false;
	mDidChow = true;

	return mDidChow;
}

bool CPlayer::Dump(CDeck &pDeck)
{
	if(!mDidGetNewCard && !mDidChow)
		return false;

	if(!pDeck.DumpACard(mIdx))
		return false;

	if(mHasSets)
		mCanCall = true;

	pDeck.Nasapawan[mIdx] = false;
	mDidGetNewCard = mDidChow = false;

	return true;
}

bool CPlayer::Show(CDeck &pDeck, int pRgn)
{
	if(!mDidGetNewCard && !mDidChow)
		return false;

	if(!pDeck.Show(mIdx, pRgn))
		return false;

	mHasSets = true;

	return true;
}

bool CPlayer::Sapaw(CDeck &pDeck, int pRgn)
{
	if(!mDidGetNewCard && !mDidChow)
		return false;

	if(!pDeck.Sapaw(mIdx, pRgn))	//Set in CDeck the 'can call' mechanism
		return false;

	return true;
}

bool CPlayer::AutoSapaw(CDeck &pDeck)
{
	if(!mDidGetNewCard && !mDidChow)
		return false;

	if(!pDeck.AutoSapaw(mIdx))
		return false;

	return true;
}

bool CPlayer::Call(CDeck &pDeck)
{
	pDeck.MakeACall();
}

void CPlayer::StartTurn(CDeck &pDeck)
{
	SDL_ShowCursor(0);

	SDL_Delay(10);


//Chow, Call or Get a new card
	if(pDeck.CanChow(mIdx))
		this->Chow(pDeck);

	else if(pDeck.mAggressiveRobot && this->CanCall() && !pDeck.Nasapawan[mIdx]) {

		SDL_Delay(10);
  		SDL_ShowCursor(1);
		this->Call(pDeck);
		return;
	}

	else
		this->GetNewCard(pDeck);


	if(pDeck.HandIsEmpty(mIdx)) {
	
		SDL_Delay(10);
  		SDL_ShowCursor(1);
		return;
	}

	SDL_Delay(10);

//Search for valid sets, then show
	while(pDeck.FindValidSet(mIdx)) {

		Show(pDeck, -1);
	}

	if(pDeck.HandIsEmpty(mIdx)) {
	
		SDL_Delay(10);
		SDL_ShowCursor(1);
		return;
	}

	SDL_Delay(10);

//Try to sapaw
	while(AutoSapaw(pDeck)) {}

	if(pDeck.HandIsEmpty(mIdx)) {
	
		SDL_Delay(10);
  		SDL_ShowCursor(1);
		return;
	}

	SDL_Delay(10);

//Dump a card, end turn
	pDeck.FindHighestRank(mIdx);
	Dump(pDeck);

	SDL_Delay(1000);

	SDL_ShowCursor(1);
}

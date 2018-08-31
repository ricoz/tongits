#include "../includes/CDeck.h"
#include <stdio.h>

int const TEMPSTART	= 5;
int const MAXTEMP	= 4;
int const SETSTART	= TEMPSTART + MAXTEMP;
int const MAXSET	= 3;

void CDeck::Init(SDL_Surface *pSurface)
{
	mACallWasMade = Nasapawan[0] = Nasapawan[1] = Nasapawan[2] = false;
	mScreen = pSurface;
	mGame.Initialize(pSurface);

	//Create the regions
	mGame.CreateRegion(  CRD_NEW, CRD_VISIBLE|CRD_3D,                                   0,              0, CRD_NSYMBOL, 544,          189,  0, 0);
	mGame.CreateRegion( CRD_DUMP, CRD_VISIBLE|CRD_FACEUP|CRD_3D,                        0,              0, CRD_NSYMBOL, 292,          189,  0, 0);
	mGame.CreateRegion(CRD_ROBOT, CRD_VISIBLE,                                          0,              0, CRD_NSYMBOL,  17,           11, 14, 0);
	mGame.CreateRegion(CRD_ROBOT, CRD_VISIBLE,                                          0,              0, CRD_NSYMBOL, 339,           11, 14, 0);
	mGame.CreateRegion(CRD_HUMAN, CRD_VISIBLE|CRD_FACEUP|CRD_DODRAG|CRD_DODROP, CRD_DOALL,  CRD_DRAGCARDS, CRD_NSYMBOL,  17,          290, 14, 0);
	mGame.CreateRegion( CRD_TEMP, CRD_VISIBLE|CRD_FACEUP|CRD_DODRAG|CRD_DODROP, CRD_DOALL, CRD_DRAGSTACKS, CRD_NSYMBOL, 264,          290, 14, 0);
	mGame.CreateRegion( CRD_TEMP, CRD_VISIBLE|CRD_FACEUP|CRD_DODRAG|CRD_DODROP, CRD_DOALL, CRD_DRAGSTACKS, CRD_NSYMBOL, 382,          290, 14, 0);
	mGame.CreateRegion( CRD_TEMP, CRD_VISIBLE|CRD_FACEUP|CRD_DODRAG|CRD_DODROP, CRD_DOALL, CRD_DRAGSTACKS, CRD_NSYMBOL, 264, 290 + 78 + 2, 14, 0);
	mGame.CreateRegion( CRD_TEMP, CRD_VISIBLE|CRD_FACEUP|CRD_DODRAG|CRD_DODROP, CRD_DOALL, CRD_DRAGSTACKS, CRD_NSYMBOL, 382, 290 + 78 + 2, 14, 0);
	//Shown sets
	mGame.CreateRegion(  CRD_SET, CRD_VISIBLE|CRD_FACEUP,                               0,              0, CRD_NSYMBOL,  17,  11 + 78 + 2, 14, 0);
	mGame.CreateRegion(  CRD_SET, CRD_VISIBLE|CRD_FACEUP,                               0,              0, CRD_NSYMBOL, 339,  11 + 78 + 2, 14, 0);
	mGame.CreateRegion(  CRD_SET, CRD_VISIBLE|CRD_FACEUP,                               0,              0, CRD_NSYMBOL,  17, 290 + 78 + 2, 14, 0);
	//transfer pile
	mGame.CreateRegion(CRD_TRANSFER, CRD_VISIBLE|CRD_FACEUP|CRD_DODRAG|CRD_3D, CRD_DOALL, 0, CRD_NSYMBOL, 292 + CARDWIDTH + 20, 189, 0, 0);

	NewCardPile		= &mGame[CRD_NEWCARDPILE];
	DumpPile		= &mGame[CRD_DUMPPILE];
	PlayerHand[0]	= &mGame[CRD_ROBOT1];
	PlayerHand[1]	= &mGame[CRD_ROBOT2];
	PlayerHand[2]	= &mGame[CRD_HUMANPLAYER];

	//NEW: set width and height of player hand
	PlayerHand[2]->SetStackWidth(224);
	PlayerHand[2]->SetStackHeight(CARDHEIGHT);
	
	TransferPile	= &mGame[CRD_TRANSFERPILE];

	int j = TEMPSTART;
	for(int i = 0; i < MAXTEMP; i++) {

		TempPile[i] = &mGame[j];
		j++;
	}

	int k = SETSTART;
	for(int i = 0; i < MAXSET; i++) {

		ValidSet[i] = &mGame[k];
		k++;
	}

	ValidSet[2]->SetStackWidth(224);
	ValidSet[2]->SetStackHeight(CARDHEIGHT);

}

bool CDeck::VerifySet(CCardStack *pStack)
{
	pStack->Sort();

	if(pStack->Size() < 3)
		return false;

	for(VI vi = pStack->begin(); vi != pStack->end(); ++vi) {

		if(vi == pStack->end() - 1)
			return true;		//same ranks means valid set

		if(vi->Rank() != (vi + 1)->Rank())
			break;				//different rank, exit loop
	}

	for(VI vi = pStack->begin(); vi != pStack->end(); ++vi) {

		if(vi == pStack->end() - 1)
			break;//same suits, exit loop

   		if( vi->Suit() != (vi + 1)->Suit() )
			return false;//different suits, not a valid set
  	}

	int rank;
	for(VI vi = pStack->begin(); vi != pStack->end(); ++vi) {

  		if(vi == pStack->end() - 1)
			return true;//ascending rank, valid set

		rank = (vi + 1)->Rank () - vi->Rank();

		if( rank != 1 )
			return false;//invalid set
	}
}

int CDeck::GetEmptyStack(int const pIdx)
{
	for(int i = 0; i < 6; i++) {

		if(Sets[pIdx][i] == NULL || Sets[pIdx][i]->Empty())
			return i;
	}

	//none was found, although this should not happen, in case this happens, increase ValidSet's size
	return -1;
}

bool CDeck::GetNewCard(int const pIdx)
{
	if(NewCardPile->Empty())
		return false;

	CCardStack *cs = new CCardStack;
	*cs = NewCardPile->Pop(1);

	mGame.InitDrag(cs, PlayerHand[pIdx], -1 , -1);
	mGame.DoDrop(PlayerHand[pIdx]);

	PlayerHand[pIdx]->InitCardFaces();
	mGame.DrawStaticScene();
	return true;
}

bool CDeck::Chow(int const pIdx)
{
	if(PlayerHand[pIdx]->GetNumSelectedCards() < 2)
		return false; //too few selected cards
	
	if(DumpPile->Empty())
		return false;
		
	if(!TransferPile->Empty())
		return false;

	CCard thrown = DumpPile->Pop(); //push again if unsuccessful

	CCardStack *cs = new CCardStack;
	cs->Push(thrown);
	cs->Push(PlayerHand[pIdx]->GetSelectedCards());

	if(VerifySet(cs)) {

		cs->Clear();
		cs->Push(thrown);
		cs->Push(PlayerHand[pIdx]->PopSelectedCards());
		cs->SetCardsFaceUp(true);
		cs->Sort();

		Sets[pIdx][this->GetEmptyStack(pIdx)] = cs;

		mGame.InitDrag(cs, PlayerHand[pIdx], -1 , -1);
		mGame.DoDrop(ValidSet[pIdx]);

		return true;
	}
	else {
		DumpPile->Push(thrown);
		DumpPile->InitCardCoords();
		PlayerHand[pIdx]->InitCardCoords();
		return false;
	}
}

bool CDeck::DumpACard(int const pIdx)
{
	if( (PlayerHand[pIdx]->GetNumSelectedCards() > 1) || (PlayerHand[pIdx]->GetNumSelectedCards() < 1) )
		return false; //too many or too few cards to dump

	CCardStack *cs = new CCardStack;
	cs->Push(PlayerHand[pIdx]->PopSelectedCards());
	cs->SetCardsFaceUp(true);

	//Doesn't refresh the card locations for robots so we should manually refresh
	PlayerHand[pIdx]->InitCardCoords();
	mGame.InitDrag(cs, PlayerHand[pIdx], -1 , -1);
	mGame.DoDrop(DumpPile);

	return true;
}

bool CDeck::Show(int const pIdx, int pRgn)
{
	if(PlayerHand[pIdx]->GetNumSelectedCards() < 3)
		return false; //too few selected cards

	if(pRgn != -1) {

		pRgn -= SETSTART;
		if(pIdx != pRgn) return false; //should click on your own sets
	}

	CCardStack *cs = new CCardStack;
	cs->Push(PlayerHand[pIdx]->GetSelectedCards());

	if(VerifySet(cs)) {

		cs->Clear();
		cs->Push(PlayerHand[pIdx]->PopSelectedCards());
		cs->SetCardsFaceUp(true);
		cs->Sort();

		Sets[pIdx][this->GetEmptyStack(pIdx)] = cs;

		mGame.InitDrag(cs, PlayerHand[pIdx], -1 , -1);
		mGame.DoDrop(ValidSet[pIdx]);

		return true;
	}
	else {
		PlayerHand[pIdx]->InitCardCoords();
		return false;
	}
}

bool CDeck::CanChow(int const pIdx)
{
	if(DumpPile->Empty())
		return false;

	CCard thrown = DumpPile->Pop();
	DumpPile->Push(thrown); //just checking so push again

	int Rank = thrown.Rank();
	int Suit = thrown.Suit();

	int a = PlayerHand[pIdx]->FindCard(Rank + 1, Suit);
	int b = PlayerHand[pIdx]->FindCard(Rank + 2, Suit);
	int c = PlayerHand[pIdx]->FindCard(Rank - 1, Suit);
	int d = PlayerHand[pIdx]->FindCard(Rank - 2, Suit);

	int e[3] = {-1, -1, -1};	//cards with same rank
	e[0] = PlayerHand[pIdx]->FindMoreSameRank(0, Rank, Suit);
	if(e[0] != -1) {

		e[1] = PlayerHand[pIdx]->FindMoreSameRank(e[0] + 1, Rank, Suit);
		if(e[1] != -1)
			e[2] = PlayerHand[pIdx]->FindMoreSameRank(e[1] + 1, Rank, Suit);
	}

	if(a != -1 && b != -1) {

		PlayerHand[pIdx]->SelectCard(a);
		PlayerHand[pIdx]->SelectCard(b);
		return true;
	}

	if(a != -1 && c != -1) {

		PlayerHand[pIdx]->SelectCard(a);
		PlayerHand[pIdx]->SelectCard(c);
		return true;
	}

	if(c != -1 && d != -1) {

		PlayerHand[pIdx]->SelectCard(c);
		PlayerHand[pIdx]->SelectCard(d);
		return true;
	}

	if(e[0] != -1 && e[1] != -1) {

		PlayerHand[pIdx]->SelectCard(e[0]);
		PlayerHand[pIdx]->SelectCard(e[1]);
		if(e[2] != -1)
			PlayerHand[pIdx]->SelectCard(e[2]);
		return true;
	}

	return false;
}

bool CDeck::FindValidSet(int const pIdx)
{
	if(PlayerHand[pIdx]->GetNumSelectedCards() > 0)
		return false; //should not select any

	CCardStack *Stack = PlayerHand[pIdx]->GetCardStack();

	int Idx = 0;
	for(VI vi = Stack->begin(); vi != Stack->end(); ++vi) {

		SDL_PumpEvents();
		int Rank = vi->Rank();
		int Suit = vi->Suit();

		int a = PlayerHand[pIdx]->FindCard(Rank + 1, Suit);
		int b = PlayerHand[pIdx]->FindCard(Rank + 2, Suit);
		int c = PlayerHand[pIdx]->FindCard(Rank - 1, Suit);
		int d = PlayerHand[pIdx]->FindCard(Rank - 2, Suit);

		int e[3];	//cards with same suit
		e[0] = PlayerHand[pIdx]->FindMoreSameRank(0, Rank, Suit);
		if(e[0] != -1) {

			e[1] = PlayerHand[pIdx]->FindMoreSameRank(e[0] + 1, Rank, Suit);
			if(e[1] != -1)
				e[2] = PlayerHand[pIdx]->FindMoreSameRank(e[1] + 1, Rank, Suit);
		}

		if(a != -1 && b != -1) {

			PlayerHand[pIdx]->SelectCard(Idx);
			PlayerHand[pIdx]->SelectCard(a);
			PlayerHand[pIdx]->SelectCard(b);
			return true;
		}

		if(a != -1 && c != -1) {

			PlayerHand[pIdx]->SelectCard(Idx);
			PlayerHand[pIdx]->SelectCard(a);
			PlayerHand[pIdx]->SelectCard(c);
			return true;
		}

		if(c != -1 && d != -1) {

			PlayerHand[pIdx]->SelectCard(Idx);
			PlayerHand[pIdx]->SelectCard(c);
			PlayerHand[pIdx]->SelectCard(d);
			return true;
		}

		if(e[0] != -1 && e[1] != -1) {

			PlayerHand[pIdx]->SelectCard(Idx);
			PlayerHand[pIdx]->SelectCard(e[0]);
			PlayerHand[pIdx]->SelectCard(e[1]);
			if(e[2] != -1)
				PlayerHand[pIdx]->SelectCard(e[2]);
			return true;
		}

		Idx++;
	}//end of for loop
	return false;
}

bool CDeck::AutoSapaw(int const pIdx)
{
	//This disables strategy, the player may want to choose where to sapaw
	//in order to inhibit his opponents from 'calling'

	CCardStack *Stack = PlayerHand[pIdx]->GetCardStack();

	int Idx = 0;
	//checks all the cards, performs sapaw until not able to do so
	for(VI vi = Stack->begin(); vi != Stack->end(); ++vi) {

		PlayerHand[pIdx]->SelectCard(Idx);

		CCardStack *cs = new CCardStack;

		for(int i = 0; i < 3; i++) {

			for(int j = 0; j < 6; j++) {

				SDL_PumpEvents();
				if((Sets[i][j] != NULL) && !Sets[i][j]->Empty()) {

					cs->Clear();
					cs->Push(PlayerHand[pIdx]->GetSelectedCards());
					CCardStack set = *Sets[i][j];
					cs->Push(set);

					if(VerifySet(cs)) {

						cs->Clear();
						cs->Push(PlayerHand[pIdx]->PopSelectedCards());
						cs->SetCardsFaceUp(true);

						Sets[i][j]->Push(*cs);
						Sets[i][j]->Sort();

						mGame.InitDrag(cs, PlayerHand[pIdx], -1 , -1);
						mGame.DoDrop(ValidSet[i]);

						ValidSet[i]->Clear();
						ValidSet[i]->Push(ExtractSet(i));
						ValidSet[i]->InitCardCoords();

						mGame.DrawStaticScene();

						if(i != pIdx)
							Nasapawan[i] = true;//CALL

      					return true;
					}
				}
			}
		}

		PlayerHand[pIdx]->InitCardCoords(); //select a different card
		Idx++;
		delete cs;

	}//end of for loop

	PlayerHand[pIdx]->InitCardCoords();
	return false;
}

void CDeck::FindHighestRank(int const pIdx)
{
	PlayerHand[pIdx]->Sort();
	PlayerHand[pIdx]->SelectCard(PlayerHand[pIdx]->Size() - 1);
}

bool CDeck::Sapaw(int const pIdx, int pRgn)
{
	if(PlayerHand[pIdx]->GetNumSelectedCards() < 1)
		return false; //too few selected cards

	pRgn -= SETSTART;

	CCardStack *cs = new CCardStack;

	for(int j = 0; j < 6; j++) {

		SDL_PumpEvents();

		if((Sets[pRgn][j] != NULL) && !Sets[pRgn][j]->Empty()) {

			cs->Clear();
			cs->Push(PlayerHand[pIdx]->GetSelectedCards());
			CCardStack set = *Sets[pRgn][j];
			cs->Push(set);

			if(VerifySet(cs)) {

				cs->Clear();
				cs->Push(PlayerHand[pIdx]->PopSelectedCards());
				cs->SetCardsFaceUp(true);

				Sets[pRgn][j]->Push(*cs);
				Sets[pRgn][j]->Sort();

				mGame.InitDrag(cs, PlayerHand[pIdx], -1 , -1);
				mGame.DoDrop(ValidSet[pRgn]);

				ValidSet[pRgn]->Clear();
				ValidSet[pRgn]->Push(ExtractSet(pRgn));
				ValidSet[pRgn]->InitCardCoords();

				mGame.DrawStaticScene();

				if(pRgn != pIdx)
					Nasapawan[pRgn] = true;//CALL

				return true;
			}
		}
	}

	delete cs;
	return false;
}

CCardStack CDeck::ExtractSet(int const pIdx)
{
	CCardStack set;

	for(int j = 0; j < 6; j++) {

		if(Sets[pIdx][j] != NULL)
			set.Push(*Sets[pIdx][j]);
	}

	return set;
}

void CDeck::ClearSets()
{
	mACallWasMade = Nasapawan[0] = Nasapawan[1] = Nasapawan[2] = false;

	for(int i = 0; i < 3; i++) {

		for(int j = 0; j < 6; j++) {

			Sets[i][j] = 0;
//			if(Sets[i][j] != NULL)
//				Sets[i][j]->Clear();
		}
	}
}

bool CDeck::ShowTemp(int pRgn)
{
	int const Human = 2;
	pRgn -= TEMPSTART;

	if(TempPile[pRgn]->Empty() || TempPile[pRgn]->Size() < 3)
		return false;

	CCardStack *cs = new CCardStack;
	*cs = *(TempPile[pRgn]->GetCardStack());

	if(VerifySet(cs)) {

		cs->SetCardsFaceUp(true);

		TempPile[pRgn]->Clear();
		Sets[Human][this->GetEmptyStack(Human)] = cs;

		mGame.InitDrag(cs, PlayerHand[Human], -1 , -1);
		mGame.DoDrop(ValidSet[Human]);
		
		return true;
	}
	else {

		delete cs;
		return false;
	}
}

bool CDeck::TempAreEmpty()
{
	for(int i = 0; i < MAXTEMP; i++) {

		if(!TempPile[i]->Empty())
			return false;
	}

	return true;
}

bool CDeck::HandIsEmpty(int const pIdx)
{
	int const Human = 2;

	if(pIdx == Human)
		return PlayerHand[pIdx]->Empty() && this->TempAreEmpty();

	else
		return PlayerHand[pIdx]->Empty();
}

int CDeck::GetTempRankValue()
{
	int total = 0;

	for(int i = 0; i < MAXTEMP; i++)
		total += TempPile[i]->GetTotalRankValue();

	return total;
}

int CDeck::GetLowestHand(int LastBunot)
{
	int const Human = 2;

//	int loval = PlayerHand[0]->GetTotalRankValue();
	int loval = 1000;
	int Idx = 0;
	int val = 0;

//hopefully this would fix the tied score problem
	int last = LastBunot;
	int mid = 0;
	
	if(last == 2) mid = 1;
	if(last == 1) mid = 0;
	if(last == 0) mid = 2;
	
	for(int j = 0; j < 3; j++) {

		if(j == Human)
			val = PlayerHand[Human]->GetTotalRankValue() + this->GetTempRankValue();

		else
			val = PlayerHand[j]->GetTotalRankValue();

		if(val == loval && j == last) {

			loval = val;
			Idx = j;
		} else
		if(val == loval && j == mid && j - 1 != last) {

			loval = val;
			Idx = j;
		} else
		if(val < loval) {

			loval = val;
			Idx = j;
		}
	}

	return Idx + 2;
}



void CDeck::RevealDump()
{
	CCardStack *cs = new CCardStack;
	if(DumpPile->Empty() && !TransferPile->Empty()) //Bring back the cards
	{
		*cs = TransferPile->Pop(TransferPile->Size());
		mGame.InitDrag(cs, DumpPile, -1, -1);
		mGame.DoDrop(DumpPile);
		DumpPile->Reverse();
		DumpPile->InitCardCoords();
	}

	else if(!DumpPile->Empty() && (!TransferPile->Empty() || TransferPile->Empty()))
	{
		*cs = DumpPile->Pop(1);
		mGame.InitDrag(cs, DumpPile, -1, -1);
		mGame.DoDrop(TransferPile);
	}
	
	mGame.DrawStaticScene();
	delete cs;
}



void CDeck::ResetDump()
{
	if(TransferPile->Empty())
		return;

	CCardStack *cs = new CCardStack;

	*cs = TransferPile->Pop(TransferPile->Size());
	mGame.InitDrag(cs, DumpPile, -1, -1);
	mGame.DoDrop(DumpPile);
	DumpPile->Reverse();
	DumpPile->InitCardCoords();
	
	mGame.DrawStaticScene();
	delete cs;
}

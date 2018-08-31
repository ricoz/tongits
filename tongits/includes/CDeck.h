#ifndef CDECK_H
#define CDECK_H

#include "../../engine/includes/CGame.h"

//Region Id's
#define		CRD_HUMAN			0
#define		CRD_ROBOT			1
#define		CRD_TEMP			2
#define		CRD_SET				3
#define		CRD_NEW  			4
#define		CRD_DUMP			5
#define		CRD_TRANSFER		6

//Region array index's
#define		CRD_NEWCARDPILE		0
#define		CRD_DUMPPILE		1
#define		CRD_ROBOT1			2
#define		CRD_ROBOT2			3
#define		CRD_HUMANPLAYER		4

#define		CRD_TRANSFERPILE	12
class CDeck
{
public:
	void Init(SDL_Surface *pSurface);

	void RevealDump();
	void ResetDump();
/*--------------------------------------------------------------------------------------------
HAND ROUTINES
--------------------------------------------------------------------------------------------*/
	bool HandIsEmpty(int const pIdx);
	bool TempAreEmpty();
	int  GetTempRankValue();
	int  GetLowestHand(int LastBunot);
	bool NewCardPileIsEmpty()			{ return NewCardPile->Empty(); }

	bool GetNewCard(int const pIdx);
	bool Chow(int const pIdx);
	bool DumpACard(int const pIdx);
	bool Sapaw(int const pIdx, int pRgn);

	bool Show(int const pIdx, int pRgn);
	bool ShowTemp(int pRgn);
/*--------------------------------------------------------------------------------------------
VALID SET ROUTINES
--------------------------------------------------------------------------------------------*/
	bool VerifySet(CCardStack *pStack);
	int  GetEmptyStack(int const pIdx);
	CCardStack ExtractSet(int const pIdx);
	void ClearSets();
/*--------------------------------------------------------------------------------------------
AI ROUTINES
--------------------------------------------------------------------------------------------*/
	bool CanChow(int const pIdx);
	bool FindValidSet(int const pIdx);
	bool AutoSapaw(int const pIdx);
	void FindHighestRank(int const pIdx);

	void MakeACall()			{ mACallWasMade = true; }
	bool ACallWasMade()			{ return mACallWasMade; }

private:
	CCardRegion		*NewCardPile;
	CCardRegion		*DumpPile;
	CCardRegion		*PlayerHand[3];
	CCardRegion		*ValidSet[3];
	CCardRegion		*TempPile[4];
	CCardRegion		*TransferPile;
	CCardStack		*Sets[3][6];

	SDL_Surface		*mScreen;

	bool			mACallWasMade;

public:
	CGame			mGame;
	bool			Nasapawan[3];		//whether a 'sapaw' was performed on a valid set, NTS: find a cleaner way
	bool			mAggressiveRobot;	//if true, always 'calls' if it can, if false, never 'calls'
};

#endif //CDECK_H

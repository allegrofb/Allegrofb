#ifndef MODELBUBBLE_H
#define MODELBUBBLE_H


#define MAXROW 12
#define MAXCOL  8

class ModelBubble
{

public:
	void Load();
	void PushDown();
	bool CheckOutside();	//11 row is outside
	bool CheckCollision(int row,int col);

private:
	int bubbleArray[MAXROW][MAXCOL];

};


  public FrozenGame(BmpWrap background_arg,
                    BmpWrap[] bubbles_arg,
                    BmpWrap[] bubblesBlind_arg,
                    BmpWrap[] frozenBubbles_arg,
                    BmpWrap[] targetedBubbles_arg,
                    BmpWrap bubbleBlink_arg,
                    BmpWrap gameWon_arg,
                    BmpWrap gameLost_arg,
                    BmpWrap hurry_arg,
                    BmpWrap penguins_arg,
                    BmpWrap compressorHead_arg,
                    BmpWrap compressor_arg,
                    Drawable launcher_arg,
                    SoundManager soundManager_arg,
                    LevelManager levelManager_arg)
  {
    random = new Random(System.currentTimeMillis());
    launcher = launcher_arg;
    penguins = penguins_arg;
    background = background_arg;
    bubbles = bubbles_arg;
    bubblesBlind = bubblesBlind_arg;
    frozenBubbles = frozenBubbles_arg;
    targetedBubbles = targetedBubbles_arg;
    bubbleBlink = bubbleBlink_arg;
    gameWon = gameWon_arg;
    gameLost = gameLost_arg;
    soundManager = soundManager_arg;
    levelManager = levelManager_arg;

    launchBubblePosition = 20; //hyjiang for test, direction, 1, 20, 39

    penguin = new PenguinSprite(penguins_arg, random);
    this.addSprite(penguin);
    compressor = new Compressor(compressorHead_arg, compressor_arg);

    hurrySprite = new ImageSprite(new Rect(203, 265, 203 + 240, 265 + 90),
                                  hurry_arg);

    jumping = new Vector();
    falling = new Vector();

    bubblePlay = new BubbleSprite[8][13];

    bubbleManager = new BubbleManager(bubbles);
    byte[][] currentLevel = levelManager.getCurrentLevel();	//hyjiang for test, get current level

    if (currentLevel == null) {
      //Log.i("frozen-bubble", "Level not available.");
      return;
    }

    //hyjiang for test, initialize bubble when game start
    for (int j=0 ; j<12 ; j++) {
      for (int i=j%2 ; i<8 ; i++) {
        if (currentLevel[i][j] != -1) {
          BubbleSprite newOne = new BubbleSprite(
               new Rect(190+i*32-(j%2)*16, 44+j*28, 32, 32),//hyjiang, bubble start position
               currentLevel[i][j],
               bubbles[currentLevel[i][j]], bubblesBlind[currentLevel[i][j]],
               frozenBubbles[currentLevel[i][j]], bubbleBlink, bubbleManager,
               soundManager, this);
          bubblePlay[i][j] = newOne;
          this.addSprite(newOne);
        }
      }
    }

    currentColor = bubbleManager.nextBubbleIndex(random);
    nextColor = bubbleManager.nextBubbleIndex(random);

    if (FrozenBubble.getMode() == FrozenBubble.GAME_NORMAL) {
      nextBubble = new ImageSprite(new Rect(302, 440, 302 + 32, 440 + 32),
                                   bubbles[nextColor]);
    } else {
      nextBubble = new ImageSprite(new Rect(302, 440, 302 + 32, 440 + 32),
                                   bubblesBlind[nextColor]);
    }
    this.addSprite(nextBubble);

    launchBubble = new LaunchBubbleSprite(currentColor, 
                                          (int)launchBubblePosition,
                                          launcher, bubbles, bubblesBlind);

    this.spriteToBack(launchBubble);

    nbBubbles = 0;
  }



#endif	//MODELBUBBLE_H


#include "macros/btlcmd.inc"

_000:
    CompareMonDataToValue OPCODE_FLAG_NOT, BTLSCR_MSG_TEMP, BATTLEMON_VOLATILE_STATUS, VOLATILE_CONDITION_SUBSTITUTE, _016
    PlayBattleAnimation BTLSCR_MSG_TEMP, BATTLE_ANIMATION_SUB_OUT
    Wait 
    RestoreSprite BTLSCR_MSG_TEMP
    Wait 
    PlayBattleAnimation BTLSCR_MSG_TEMP, BATTLE_ANIMATION_SUB_IN
    Wait 

_016:
    // {0}’s {1} is reacting to {2}’s Mega Bracelet!
    PrintMessage pl_msg_00000368_01269, TAG_NICKNAME_ITEM_TRNAME, BTLSCR_MSG_TEMP, BTLSCR_MSG_TEMP, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    PlayBattleAnimation BTLSCR_MSG_TEMP, BATTLE_ANIMATION_MEGA_EVOLUTION
    Wait
    ChangeForm BTLSCR_MSG_TEMP
    // {0} mega evolved!
    PrintMessage pl_msg_00000368_01272, TAG_NICKNAME, BTLSCR_MSG_TEMP
    Wait  
    WaitButtonABTime 30
    CompareMonDataToValue OPCODE_FLAG_NOT, BTLSCR_MSG_TEMP, BATTLEMON_VOLATILE_STATUS, VOLATILE_CONDITION_SUBSTITUTE, _058
    PlayBattleAnimation BTLSCR_MSG_TEMP, BATTLE_ANIMATION_SUB_OUT
    Wait 
    RefreshSprite BTLSCR_MSG_TEMP
    Wait 
    PlayBattleAnimation BTLSCR_MSG_TEMP, BATTLE_ANIMATION_SUB_IN
    Wait 

_058:
    End 

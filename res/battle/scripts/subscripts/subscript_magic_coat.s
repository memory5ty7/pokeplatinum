#include "macros/btlcmd.inc"

    .data

_000:
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 15
    CheckAbility CHECK_HAVE, BTLSCR_DEFENDER, ABILITY_MAGIC_BOUNCE, _015
    // {0}’s {1} was bounced back by Magic Coat!
    PrintMessage pl_msg_00000368_00574, TAG_NICKNAME_MOVE, BTLSCR_ATTACKER, BTLSCR_ATTACKER
    GoTo _020

_015:
    // {0}’s {1} was bounced back by Magic Bounce!
    PrintMessage pl_msg_00000368_00574, TAG_NICKNAME_MOVE, BTLSCR_ATTACKER, BTLSCR_ATTACKER

_020:
    Wait 
    WaitButtonABTime 30
    MagicCoat 
    UpdateVar OPCODE_FLAG_OFF, BTLVAR_BATTLE_CTX_STATUS, SYSCTL_PLAYED_MOVE_ANIMATION
    End 
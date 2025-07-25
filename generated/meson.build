# This build file defines sets of constants which are used throughout the code
# base, both in the ROM's source code and in internal tooling. We generate these
# constants to ensure that they have a particular set of characteristics, are
# easily validated in data files representing them as strings, and can be easily
# parsed by simple string manipulations.

# This must be declared here instead of in tools/meson.build, as some tools depend
# on metang-generated constants.
subproject('metang')

metang_exe = find_program('metang', native: true)

metang_generators = {
    'abilities': { 'type': 'enum', 'tag': 'Ability' },
    'ai_action_choices': { 'type': 'enum', 'tag': 'AIActionChoice' },
    'ai_flags': { 'type': 'mask', 'tag': 'AIFlag', 'extra': ['--no-auto'] },
    'ai_load_type_targets': { 'type': 'enum', 'tag': 'AILoadTypeTarget' },
    'ai_weather_types': { 'type': 'enum', 'tag': 'AIWeatherType' },
    'badges': { 'type': 'enum', 'tag': 'Badge' },
    'battle_actions': { 'type': 'enum', 'tag': 'BattleAction' },
    'battle_backgrounds': { 'type': 'enum', 'tag': 'BattleBackground' },
    'battle_boot_states': { 'type': 'enum', 'tag': 'BattleBootState' },
    'battle_context_params': { 'type': 'enum', 'tag': 'BattleContextParam' },
    'battle_message_tags': { 'type': 'enum', 'tag': 'BattleMessageTag' },
    'battle_mon_params': { 'type': 'enum', 'tag': 'BattleMonParam' },
    'battle_move_effects': { 'type': 'enum', 'tag': 'BattleMoveEffect' },
    'battle_move_subscript_ptrs': { 'type': 'enum', 'tag': 'BattleMoveSubscriptPtr' },
    'battle_script_battlers': { 'type': 'enum', 'tag': 'Battler' },
    'battle_script_check_side_condition_ops': { 'type': 'enum', 'tag': 'BattleScriptCheckSideConditionOp' },
    'battle_script_opcodes': { 'type': 'enum', 'tag': 'BattleScriptOpCode' },
    'battle_script_side_conditions': { 'type': 'enum', 'tag': 'BattleScriptSideCondition' },
    'battle_script_turn_flags': { 'type': 'enum', 'tag': 'BattleScriptTurnFlag' },
    'battle_script_vars': { 'type': 'enum', 'tag': 'BattleScriptVars' },
    'battle_side_effect_types': { 'type': 'enum', 'tag': 'BattleSideEffectType' },
    'battle_stats': { 'type': 'enum', 'tag': 'BattleStat' },
    'battle_sub_animations': { 'type': 'enum', 'tag': 'BattleSubAnimation' },
    'battle_subscripts': { 'type': 'enum', 'tag': 'BattleSubscript' }, # functionally pre-generating the NARC index
    'battle_terrains': { 'type': 'enum', 'tag': 'BattleTerrain' },
    'catching_show_points_category': { 'type': 'enum', 'tag': 'CatchingShowPointsCategory' },
    'egg_groups': { 'type': 'enum', 'tag': 'EggGroup' },
    'evolution_methods': { 'type': 'enum', 'tag': 'EvolutionMethod' },
    'exp_rates': { 'type': 'enum', 'tag': 'ExpRate' },
    'game_records': { 'type': 'enum', 'tag': 'GameRecord' },
    'footprint_sizes': { 'type': 'enum', 'tag': 'FootprintSize' },
    'gender_ratios': { 'type': 'enum', 'tag': 'GenderRatio' },
    'genders': { 'type': 'enum', 'tag': 'Gender' },
    'item_ai_categories': { 'type': 'enum', 'tag': 'ItemAICategory' },
    'item_battle_categories': { 'type': 'enum', 'tag': 'ItemBattleCategory' },
    'item_hold_effects': { 'type': 'enum', 'tag': 'ItemHoldEffect' },
    'items': { 'type': 'enum', 'tag': 'Item' },
    'journal_location_events': { 'type': 'enum', 'tag': 'JournalLocationEventType' },
    'journal_online_events': { 'type': 'enum', 'tag': 'JournalOnlineEventType' },
    'map_headers': { 'type': 'enum', 'tag': 'MapHeader' },
    'move_attributes': { 'type': 'enum', 'tag': 'MoveAttribute' },
    'move_classes': { 'type': 'enum', 'tag': 'MoveClass' },
    'move_flags': { 'type': 'mask', 'tag': 'MoveFlag' },
    'move_ranges': { 'type': 'mask', 'tag': 'MoveRange', 'extra': ['--no-auto'] },
    'movement_actions': { 'type': 'enum', 'tag': 'MovementAction' },
    'moves': { 'type': 'enum', 'tag': 'Move' },
    'pal_park_land_area': { 'type': 'enum', 'tag': 'PalParkLandArea' },
    'pal_park_water_area': { 'type': 'enum', 'tag': 'PalParkWaterArea' },
    'pokemon_body_shapes': { 'type': 'enum', 'tag': 'PokemonBodyShape' },
    'pokemon_colors': { 'type': 'enum', 'tag': 'PokemonColor' },
    'pokemon_contest_types': { 'type': 'enum', 'tag': 'PokemonContestType' },
    'pokemon_types': { 'type': 'enum', 'tag': 'PokemonType' },
    'poketch_apps': { 'type': 'enum', 'tag': 'PoketchAppID' },
    'save_types': { 'type': 'enum', 'tag': 'SaveType' },
    'sdat': { 'type': 'enum', 'tag': 'SDATID' },
    'shadow_sizes': { 'type': 'enum', 'tag': 'ShadowSize' },
    'species': { 'type': 'enum', 'tag': 'Species' },
    'species_data_params': { 'type': 'enum', 'tag': 'SpeciesDataParam' },
    'text_banks': { 'type': 'enum', 'tag': 'TextBank' },
    'trainers': { 'type': 'enum', 'tag': 'Trainer' },
    'trainer_classes': { 'type': 'enum', 'tag': 'TrainerClass' },
    'trainer_score_events': { 'type': 'enum', 'tag': 'TrainerScoreEvent' },
}

c_consts_generators = []
py_consts_generators = []

foreach gen_key : metang_generators.keys()
    gen_h = gen_key + '.h'
    gen_py = gen_key + '.py'
    gen_file = files(gen_key + '.txt')
    gen_args = metang_generators.get(gen_key)
    gen_extra_args = gen_args.get('extra', [])

    c_consts_generators += custom_target(gen_h,
        output: gen_h,
        input: gen_file,
        command: [
            metang_exe, gen_args['type'],
            '--tag-name', gen_args['tag'],
            '--guard', 'POKEPLATINUM_GENERATED',
            '--output', '@OUTPUT@',
            gen_extra_args,
            '@INPUT@',
        ]
    )

    py_consts_generators += custom_target(gen_py,
        output: gen_py,
        input: gen_file,
        command: [
            metang_exe, gen_args['type'],
            '--tag-name', gen_args['tag'],
            '--output', '@OUTPUT@',
            '--lang', 'py',
            gen_extra_args,
            '@INPUT@',
        ]
    )
endforeach

items_txt = files('items.txt')
species_txt = files('species.txt')
text_banks_txt = files('text_banks.txt')
trainers_txt = files('trainers.txt')

py_consts_env = environment()
py_consts_env.append('PYTHONPATH', meson.project_build_root())
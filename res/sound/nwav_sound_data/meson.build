nwav_sound_data_files = copy_gen.process(files(
    'SEQ_D_MOUNT1.swav',
))

nwav_sound_data_narc = custom_target('nwav_sound_data.narc',
    output: [
        'nwav_sound_data.narc',
    ],
    input: nwav_sound_data_files,
    command: [
        nitroarc_exe,
        '--create',
        '--files-from', files('nwav_sound_data.order'),
        '--file', '@OUTPUT0@',
        '@PRIVATE_DIR@',
    ],
)

nitrofs_files += nwav_sound_data_narc

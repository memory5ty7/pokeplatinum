subdir('areadata')
subdir('build_model')
subdir('encountdata')
subdir('eventdata')
subdir('land_data')
subdir('mapmatrix')
subdir('maptable')
subdir('mm_list')
subdir('pokemon_trade')
subdir('script')
subdir('tornworld')

prebuilt_files = [
    'scr_seq.narc'
]

foreach f : prebuilt_files
    nitrofs_files += fs.copyfile(f)
endforeach

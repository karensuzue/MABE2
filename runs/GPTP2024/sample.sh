#!/bin/tcsh

set num_runs = 10
set exe = ../../build/MABE

echo =============
echo NK Landscapes
echo =============
mkdir nk
foreach mode (withage control)
    set data_dir = nk/$mode
    mkdir $data_dir
    set config = NKAge_$mode.mabe

    foreach run_id (`seq 1 $num_runs`)
        set filename = $data_dir/run_$run_id.csv
        set run_line = ($exe -s random_seed=$run_id -f $config -s fit_file.filename=\"$filename\")
        echo $run_line
        $run_line
    end
end

echo =============
echo Diagnostics
echo =============
mkdir diagnostics
foreach mode (withage control)
    set data_dir = diagnostics/$mode
    mkdir $data_dir
    set config = DiagnosticsAge_$mode.mabe

    foreach run_id (`seq 1 $num_runs`)
        set filename = $data_dir/run_$run_id.csv
        set run_line = ($exe -s random_seed=$run_id -f $config -s fit_file.filename=\"$filename\")
        echo $run_line
        $run_line
    end
end
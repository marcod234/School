onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /sproc_tb/UUT/Clock
add wave -noupdate /sproc_tb/UUT/Reset
add wave -noupdate /sproc_tb/UUT/Start
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Ins_Addr
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Ins_In
add wave -noupdate /sproc_tb/UUT/Ins_Wren
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Data_Addr
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Data_In
add wave -noupdate /sproc_tb/UUT/Data_Wren
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/PC
add wave -noupdate /sproc_tb/UUT/IM_en
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/ctr
add wave -noupdate /sproc_tb/UUT/IM_wren
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/IM_addr
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/IM_out
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Reg_num_1
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Reg_out_1
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Reg_num_2
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Reg_out_2
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/a_in
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/b_in
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/alu_ctrl
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/ALU_out
add wave -noupdate /sproc_tb/UUT/true_flag
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/DM_addr
add wave -noupdate /sproc_tb/UUT/DM_rden
add wave -noupdate /sproc_tb/UUT/DM_wren
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/DM_out
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/Reg_in
add wave -noupdate /sproc_tb/UUT/Reg_en
add wave -noupdate /sproc_tb/UUT/ALU_en
add wave -noupdate /sproc_tb/UUT/DM_en
add wave -noupdate /sproc_tb/UUT/Reg_wr
add wave -noupdate /sproc_tb/UUT/IM/Clock
add wave -noupdate /sproc_tb/UUT/IM/Reset
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/IM/DataIn
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/IM/Address
add wave -noupdate /sproc_tb/UUT/IM/WriteEn
add wave -noupdate /sproc_tb/UUT/IM/Enable
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/IM/DataOut
add wave -noupdate /sproc_tb/UUT/REG/Clock
add wave -noupdate /sproc_tb/UUT/REG/Reset
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/REG/DataIn
add wave -noupdate /sproc_tb/UUT/REG/RegWrite
add wave -noupdate /sproc_tb/UUT/REG/Enable
add wave -noupdate /sproc_tb/UUT/REG/Reg
add wave -noupdate /sproc_tb/UUT/DM/Clock
add wave -noupdate /sproc_tb/UUT/DM/Reset
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/DM/DataIn
add wave -noupdate -radix hexadecimal /sproc_tb/UUT/DM/Address
add wave -noupdate /sproc_tb/UUT/DM/MemWrite
add wave -noupdate /sproc_tb/UUT/DM/MemRead
add wave -noupdate /sproc_tb/UUT/DM/Enable
add wave -noupdate /sproc_tb/UUT/DM/DataOut
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {154503 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 219
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {57379 ps} {260138 ps}

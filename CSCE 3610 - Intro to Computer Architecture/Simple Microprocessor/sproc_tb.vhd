-- Vhdl test bench
--
--
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY sproc_tb IS
END sproc_tb;

ARCHITECTURE behavioral OF sproc_tb IS 


  COMPONENT sproc
    GENERIC (
        DATA_WIDTH    : integer := 16;
        ADDRESS_WIDTH : integer := 16
        );
    PORT ( 
        Clock       : in  STD_LOGIC;
        Reset       : in  STD_LOGIC;
        Start       : in  STD_LOGIC;
        Ins_In      : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        Ins_Addr    : in  STD_LOGIC_VECTOR (ADDRESS_WIDTH - 1 downto 0);
        Ins_Wren     : in  STD_LOGIC;
        Data_In     : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        Data_Addr   : in  STD_LOGIC_VECTOR (ADDRESS_WIDTH - 1 downto 0);
        Data_Wren   : in  STD_LOGIC);
   END COMPONENT;

   SIGNAL clock, reset, Ins_wren, Data_wren, start : STD_LOGIC;
   SIGNAL Ins_in, Data_in                          : STD_LOGIC_VECTOR (15 downto 0);
   SIGNAL Ins_Addr, Data_Addr                      : STD_LOGIC_VECTOR (15 downto 0);
   
   CONSTANT clock_period                 : time := 10 ns;

BEGIN

   UUT: sproc
     GENERIC MAP (
                   DATA_WIDTH    => 16,
                   ADDRESS_WIDTH => 16
     )

     PORT MAP(
                  Clock       => Clock,
                  Reset       => Reset,
                  Start       => Start,
                  Ins_in      => Ins_in,
                  Ins_Addr    => Ins_Addr,
                  Ins_wren    => Ins_Wren,
                  Data_in     => Data_in,
                  Data_Addr   => Data_Addr,
                  Data_wren   => Data_Wren);

   clk: PROCESS
   BEGIN
      Clock <= '1';
	  wait for clock_period/2;
	  Clock <= '0';
	  wait for clock_period/2;
	  
   END PROCESS;
   
-- *** Test Bench - User Defined Section ***
   tb : PROCESS
   BEGIN
	
	Reset <= '1';
	wait for 2*clock_period;

	Reset    <= '0';
        start    <= '0';
        Ins_Wren <= '1';
	wait for clock_period;

-- 1st program
---------------------------------------------------------------------------------------------
--	Ins_Wren <= '1'; Ins_Addr <= x"0000"; Ins_in <= x"D001";  -- Ins Write Operation LDR
--	wait for clock_period;
	
--	Ins_Wren <= '1'; Ins_Addr <= x"0002"; Ins_in <= x"D202";  -- Ins Write Operation LDR
--	wait for clock_period;

--	Ins_Wren <= '1'; Ins_Addr <= x"0004"; Ins_in <= x"4123";  -- Ins Write Operation DIV
--	wait for clock_period;

--	Ins_Wren <= '1'; Ins_Addr <= x"0006"; Ins_in <= x"1324";  -- Ins Write Operation ADD
--	wait for clock_period;

--	Ins_Wren <= '1'; Ins_Addr <= x"0008"; Ins_in <= x"2225";  -- Ins Write Operation SUB
--	wait for clock_period;

--	Ins_Wren <= '1'; Ins_Addr <= x"000A"; Ins_in <= x"0025";  -- Ins Write Operation BEQ
--	wait for clock_period;

--	Ins_Wren <= '1'; Ins_Addr <= x"000C"; Ins_in <= x"A135";  -- Ins Write Operation SUBI
--	wait for clock_period;

--	Ins_Wren <= '1'; Ins_Addr <= x"000E"; Ins_in <= x"7025";  -- Ins Write Operation BLT
--	wait for clock_period;

--	Ins_Wren <= '1'; Ins_Addr <= x"0010"; Ins_in <= x"9326";  -- Ins Write Operation ADDI
--	wait for clock_period;

--     	Ins_Wren <= '1'; Ins_Addr <= x"0012"; Ins_in <= x"F001";  -- Ins Write Operation B
--	wait for clock_period;


-- 2nd program
-------------------------------------------------------------------------------------------------
        Ins_Wren <= '1'; Ins_Addr <= x"0000"; Ins_in <= x"D001";  -- Ins Write Operation LDR
	wait for clock_period;

        Ins_Wren <= '1'; Ins_Addr <= x"0002"; Ins_in <= x"D202";  -- Ins Write Operation LDR
	wait for clock_period;

	Ins_Wren <= '1'; Ins_Addr <= x"0004"; Ins_in <= x"3123";  -- Ins Write Operation MUL
	wait for clock_period;

	Ins_Wren <= '1'; Ins_Addr <= x"0006"; Ins_in <= x"5123";  -- Ins Write Operation AND
	wait for clock_period;

	Ins_Wren <= '1'; Ins_Addr <= x"0008"; Ins_in <= x"6123";  -- Ins Write Operation ORR
	wait for clock_period;

	Ins_Wren <= '1'; Ins_Addr <= x"000A"; Ins_in <= x"B125";  -- Ins Write Operation LSL
	wait for clock_period;

	Ins_Wren <= '1'; Ins_Addr <= x"000C"; Ins_in <= x"C135";  -- Ins Write Operation LSR
	wait for clock_period;

	Ins_Wren <= '1'; Ins_Addr <= x"000E"; Ins_in <= x"2025";  -- Ins Write Operation SUB
	wait for clock_period;

	Ins_Wren <= '1'; Ins_Addr <= x"0010"; Ins_in <= x"9015";  -- Ins Write Operation BGT
	wait for clock_period;

       	Ins_Wren <= '1'; Ins_Addr <= x"0012"; Ins_in <= x"E051";  -- Ins Write Operation STR
	wait for clock_period;

--------------------------------------------------------------------------------------------------------

	Ins_Wren <= '0'; Ins_Addr <= x"0000";                     -- Ins Read Operation
	wait for clock_period;

	Data_Wren <= '1'; Data_Addr <= x"0000"; Data_in <= x"000F";  -- Data Write Operation
	wait for clock_period;

	Data_Wren <= '1'; Data_Addr <= x"0002"; Data_in <= x"000F";  -- Data Write Operation
	wait for clock_period;

        start <= '1';

        Data_Wren <= '0';
	
    WAIT; -- will wait forever
	
   END PROCESS;
-- *** End Test Bench - User Defined Section ***

END;

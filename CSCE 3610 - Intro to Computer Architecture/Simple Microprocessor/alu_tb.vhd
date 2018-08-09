LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;
USE IEEE.NUMERIC_STD.ALL;
 
entity alu_tb is
end alu_tb;

ARCHITECTURE behavioral OF alu_tb IS 

  component alu 
    Generic (
          DATA_WIDTH    : integer := 16
    );
    Port ( 
        Clock       : in  STD_LOGIC;
        Reset       : in  STD_LOGIC;
        a_in        : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        b_in        : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        alu_ctrl    : in  STD_LOGIC_VECTOR (3 downto 0);
        Enable      : in  STD_LOGIC;
        true_flag   : out STD_LOGIC;
        Res_out     : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0)
    );
   End component;

   SIGNAL Clock, Reset, Enable, true_flag          : STD_LOGIC;
   SIGNAL a_in, b_in, Res_out                      : STD_LOGIC_VECTOR (15 downto 0);
   SIGNAL alu_ctrl                                 : STD_LOGIC_VECTOR (3 downto 0);
   
   CONSTANT clock_period                 : time := 10 ns;

BEGIN

  UUT: alu generic map(16) port map(Clock, Reset, a_in, b_in, alu_ctrl, Enable, true_flag, Res_out);

   clk: PROCESS
   BEGIN
      Clock <= '1';
	  wait for clock_period/2;
	  Clock <= '0';
	  wait for clock_period/2;
	  
   END PROCESS;

   tb : PROCESS
   BEGIN
     Reset <= '1';
     wait for 2*clock_period;

     Reset <='0';
     Enable <= '1';
     alu_ctrl <= x"3";
     a_in <= x"000F"; b_in <= x"000F";
     wait for clock_period;

   wait;
   end process;


end behavioral;

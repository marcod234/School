LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;
 
Entity regs is
Generic (
          DATA_WIDTH    : integer := 16;
          REGS_NUM      : integer := 4
);
Port ( 
        Clock       : in  STD_LOGIC;
        Reset       : in  STD_LOGIC;
        DataIn      : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        Reg_num_1   : in  STD_LOGIC_VECTOR (REGS_NUM - 1 downto 0);
        Reg_num_2   : in  STD_LOGIC_VECTOR (REGS_NUM - 1 downto 0);
        RegWrite    : in  STD_LOGIC;
        Enable      : in  STD_LOGIC;
        DataOut_1   : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        DataOut_2   : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0)
);
End regs;
 
Architecture Behavioral of regs is

  type Reg_Array is array ((2 ** REGS_NUM) - 1 downto 0) of STD_LOGIC_VECTOR ((DATA_WIDTH -1) downto 0);
  signal Reg : Reg_Array;
  
begin
 
-- Read process
  Process (Clock)
  begin

    if rising_edge(Clock) then

      if Reset = '1' then   -- Clear DataOut on Reset
        
        DataOut_1 <= (others => '0');
        DataOut_2 <= (others => '0');
        
        Reg(0)    <= (others => '0');
        Reg(1)    <= (others => '0');
        Reg(2)    <= (others => '0');
        Reg(3)    <= (others => '0');
        Reg(4)    <= (others => '0');
        Reg(5)    <= (others => '0');
        Reg(6)    <= (others => '0');
        Reg(7)    <= (others => '0');
        Reg(8)    <= (others => '0');
        Reg(9)    <= (others => '0');
        Reg(10)   <= (others => '0');
        Reg(11)   <= (others => '0');
        Reg(12)   <= (others => '0');
        Reg(13)   <= (others => '0');
        Reg(14)   <= (others => '0');
        Reg(15)   <= (others => '0');
		
      elsif Enable = '1' then
	  
        if RegWrite = '1' then -- If WriteEn then pass through DIn
        
          Reg(to_integer(unsigned(Reg_num_1))) <= DataIn((DATA_WIDTH - 1) downto 0);

        else -- Otherwise Read Memory

          DataOut_1((DATA_WIDTH - 1) downto 0) <= Reg(to_integer(unsigned(Reg_num_1)));
          DataOut_2((DATA_WIDTH - 1) downto 0) <= Reg(to_integer(unsigned(Reg_num_2)));

        end if;

      end if;

    end if;
	
  end process;

End Architecture;


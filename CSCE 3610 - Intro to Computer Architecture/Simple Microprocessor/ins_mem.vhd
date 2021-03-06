LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;
 
Entity ins_mem is
Generic (
  DATA_WIDTH    : integer := 16;
  ADDRESS_WIDTH : integer := 16
);
Port ( 
        Clock       : in  STD_LOGIC;
        Reset       : in  STD_LOGIC;
        DataIn      : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        Address     : in  STD_LOGIC_VECTOR (ADDRESS_WIDTH - 1 downto 0);
        WriteEn     : in  STD_LOGIC;
        Enable      : in  STD_LOGIC;
        DataOut     : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0)
);
End ins_mem;
 
Architecture Behavioral of ins_mem is

  type Memory_Array is array ((2 ** ADDRESS_WIDTH) - 1 downto 0) of STD_LOGIC_VECTOR (7 downto 0);
  signal I_Mem : Memory_Array;
  
begin
 
-- Read process
  Process (Clock)
  begin

    if rising_edge(Clock) then

	  if Reset = '1' then   -- Clear DataOut on Reset
        
		DataOut <= (others => '0');
		
          elsif Enable = '1' then
	  
            if WriteEn = '1' then -- If WriteEn then pass through DIn
        
	      I_Mem(to_integer(unsigned(Address)))     <= DataIn(7 downto 0);
		  I_Mem(to_integer(unsigned(Address)) + 1) <= DataIn(15 downto 8);

		else -- Otherwise Read Memory

		  DataOut(7 downto 0)  <= I_Mem(to_integer(unsigned(Address)));
		  DataOut(15 downto 8) <= I_Mem(to_integer(unsigned(Address)) + 1);

        
		end if;

	  end if;

    end if;
	
  end process;

End Architecture;



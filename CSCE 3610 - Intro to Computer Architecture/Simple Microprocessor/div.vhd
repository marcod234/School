library IEEE; 
use IEEE.STD_LOGIC_1164.all; 
use IEEE.STD_LOGIC_UNSIGNED.all;  
use IEEE.STD_LOGIC_ARITH.all; 
 
entity div is  
    generic(SIZE: INTEGER := 8); 
    port(reset: in STD_LOGIC; 
        en: in STD_LOGIC; 
        clk: in STD_LOGIC; 
         
        num: in STD_LOGIC_VECTOR((SIZE - 1) downto 0); 
        den: in STD_LOGIC_VECTOR((SIZE - 1) downto 0); 
        res: out STD_LOGIC_VECTOR((SIZE - 1) downto 0); 
        rm: out STD_LOGIC_VECTOR((SIZE - 1) downto 0) 
        ); 
end div; 
 
architecture behav of div is 
    signal buf: STD_LOGIC_VECTOR((2 * SIZE - 1) downto 0); 
    signal dbuf: STD_LOGIC_VECTOR((SIZE - 1) downto 0); 
    signal sm: INTEGER range 0 to SIZE; 
     
    alias buf1 is buf((2 * SIZE - 1) downto SIZE); 
    alias buf2 is buf((SIZE - 1) downto 0); 
begin 
    p_001: process(reset, en, clk) 
    begin 
        if reset = '1' then 
            res <= (others => '0'); 
            rm <= (others => '0'); 
            sm <= 0; 
        elsif rising_edge(clk) then 
            if en = '1' then 
                case sm is 
                when 0 => 
                    buf1 <= (others => '0'); 
                    buf2 <= num; 
                    dbuf <= den; 
                    res <= buf2; 
                    rm <= buf1; 
                    sm <= sm + 1; 
                when others => 
                    if buf((2 * SIZE - 2) downto (SIZE - 1)) >= dbuf then 
                        buf1 <= '0' & (buf((2 * SIZE - 3) downto (SIZE - 1)) - dbuf((SIZE - 2) downto 0)); 
                        buf2 <= buf2((SIZE - 2) downto 0) & '1'; 
                    else 
                        buf <= buf((2 * SIZE - 2) downto 0) & '0'; 
                    end if; 
                    if sm /= SIZE then 
                        sm <= sm + 1; 
                    else 
                        sm <= 0; 
                    end if; 
                end case; 
            end if; 
        end if; 
    end process; 
end behav;
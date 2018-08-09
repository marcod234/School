LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;
USE IEEE.NUMERIC_STD.ALL;
 
Entity sproc is
Generic (
        DATA_WIDTH    : integer := 16;
        ADDRESS_WIDTH : integer := 16
);
Port ( 
        Clock       : in  STD_LOGIC;
        Reset       : in  STD_LOGIC;
        Start       : in  STD_LOGIC;
        Ins_In      : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        Ins_Addr    : in  STD_LOGIC_VECTOR (ADDRESS_WIDTH - 1 downto 0);
        Ins_Wren    : in  STD_LOGIC;
        Data_In     : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
        Data_Addr   : in  STD_LOGIC_VECTOR (ADDRESS_WIDTH - 1 downto 0);
        Data_Wren   : in  STD_LOGIC
);
End sproc;
 
Architecture Behavioral of sproc is

  signal PC                                  : STD_LOGIC_VECTOR((ADDRESS_WIDTH - 1) downto 0);
  signal IM_in, IM_out                       : STD_LOGIC_VECTOR((DATA_WIDTH - 1) downto 0);  
  signal Reg_in, Reg_out_1, Reg_out_2        : STD_LOGIC_VECTOR((DATA_WIDTH - 1) downto 0);  
  signal DM_in, DM_out, a_in, b_in, ALU_out  : STD_LOGIC_VECTOR((DATA_WIDTH - 1) downto 0);  
  signal IM_en, Reg_en, ALU_en, DM_en        : STD_LOGIC;
  signal IM_addr, DM_addr                    : STD_LOGIC_VECTOR((ADDRESS_WIDTH - 1) downto 0);  
  signal IM_wren, Reg_wr                     : STD_LOGIC;
  signal DM_wren, DM_rden, true_flag         : STD_LOGIC;
  signal Reg_num_1, Reg_num_2, alu_ctrl      : STD_LOGIC_VECTOR(3 downto 0);  
  signal ctr                                 : STD_LOGIC_VECTOR(7 downto 0);   
  signal ctr2                                : STD_LOGIC_VECTOR(11 downto 0);
  

  COMPONENT ins_mem
     GENERIC (
           DATA_WIDTH    : integer := 16;
           ADDRESS_WIDTH : integer := 16
           );
	 PORT( 
           Clock       : in  STD_LOGIC;
           Reset       : in  STD_LOGIC;
           DataIn      : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
           Address     : in  STD_LOGIC_VECTOR (ADDRESS_WIDTH - 1 downto 0);
           WriteEn     : in  STD_LOGIC;
           Enable      : in  STD_LOGIC;
           DataOut     : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0)
           );
   END COMPONENT;
   
   COMPONENT regs
     GENERIC (
               DATA_WIDTH    : integer := 16;
               REGS_NUM      : integer := 4
             );
	 PORT
             ( 
               Clock       : in  STD_LOGIC;
               Reset       : in  STD_LOGIC;
               DataIn      : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
               Reg_num_1   : in  STD_LOGIC_VECTOR (REGS_NUM - 1 downto 0);
               Reg_num_2   : in  STD_LOGIC_VECTOR (REGS_NUM - 1 downto 0);
               RegWrite    : in  STD_LOGIC;
               Enable      : in  STD_LOGIC;
               DataOut_1   : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
               DataOut_2   : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0)
           );
   END COMPONENT;
   
   COMPONENT alu
     GENERIC (
               DATA_WIDTH    : integer := 16
             );
	 PORT
             ( 
               Clock       : in  STD_LOGIC;
              Reset       : in  STD_LOGIC;
               a_in        : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
               b_in        : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
               alu_ctrl    : in  STD_LOGIC_VECTOR (3 downto 0);
               Enable      : in  STD_LOGIC;
               true_flag   : out STD_LOGIC;
               Res_out     : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0)
           );
   END COMPONENT;
   
   COMPONENT data_mem
     GENERIC (
               DATA_WIDTH    : integer := 16;
               ADDRESS_WIDTH : integer := 16
             );
	 PORT
             ( 
               Clock       : in  STD_LOGIC;
              Reset       : in  STD_LOGIC;
               DataIn      : in  STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0);
               Address     : in  STD_LOGIC_VECTOR (ADDRESS_WIDTH - 1 downto 0);
               MemWrite    : in  STD_LOGIC;
               MemRead     : in  STD_LOGIC;
               Enable      : in  STD_LOGIC;
               DataOut     : out STD_LOGIC_VECTOR (DATA_WIDTH - 1 downto 0)
           );
   END COMPONENT;
   
begin
 
-- Read process
  Process (Clock)
  begin

    if rising_edge(Clock) then

      if Reset = '1' then   -- Clear DataOut on Reset
    
        PC     <= (others => '0');    
        Reg_in <= (others => '0');    
        ctr    <= x"00";
        ctr2   <= x"000";

      else

        IM_en  <= '1';
        Reg_en <= '1';
        Alu_en <= '1';
        DM_en  <= '1';

        if (start = '0') then
          
          DM_wren <= Data_wren;
          DM_Addr <= Data_Addr;
          DM_in   <= Data_in;

        else

          DM_wren <= '0';
          DM_Addr <= (others => '0');
          DM_in   <= (others => '0');
 
          case IM_out(15 downto 12) is 

          when x"0" =>  -- Branch Equal 

            if (ctr = x"00") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(3 downto 0);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01";

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr       <= ctr + x"01";

            elsif (ctr = x"02") then -- Setting up ALU for equal compare

              a_in     <= Reg_out_1;
              b_in     <= (others => '0');
              alu_ctrl <= x"8";
              ctr       <= ctr + x"01";

            elsif (ctr = x"03") then -- Waiting for ALU to compare

              ctr       <= ctr + x"01";

            elsif (ctr = x"04") then -- Checking the true flag for jump

              if (true_flag = '1') then 
     
                PC <= PC + (x"02" * IM_out(11 downto 4)); -- The condition is true so jump

              else 
     
                PC <= PC + x"0002"; -- The condition is false so move to next instruction

              end if;

              ctr       <= ctr + x"01";

            elsif (ctr = x"05") then -- Do nothing

              ctr       <= (others => '0');

            end if;
          
          when x"1" => -- ADD Instruction
          
            if (ctr = x"00") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_num_2 <= IM_out(11 downto 8);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing Add in ALU

              a_in     <= Reg_out_1;
              b_in     <= Reg_out_2;
              alu_ctrl <= x"0";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU to add

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Writing back to Registers

              ctr       <= (others => '0');

            end if;

          when x"2" => -- SUB Instruction
          
            if (ctr = x"00") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_num_2 <= IM_out(11 downto 8);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing sub in ALU

              a_in     <= Reg_out_1;
              b_in     <= Reg_out_2;
              alu_ctrl <= x"1";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU to sub

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Writing back to Registers

              ctr       <= (others => '0');

            end if;
          
          when x"3" => -- MUL Instruction
          
            if (ctr = x"00") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_num_2 <= IM_out(11 downto 8);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing mul in ALU

              a_in     <= Reg_out_1;
              b_in     <= Reg_out_2;
              alu_ctrl <= x"2";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU to mul

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Writing back to Registers

              ctr       <= (others => '0');

            end if;
          when x"4" => -- DIV Instruction
          
            if (ctr = x"00") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_num_2 <= IM_out(11 downto 8);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing div in ALU

              a_in     <= Reg_out_1;
              b_in     <= Reg_out_2;
              alu_ctrl <= x"3";
              ctr      <= ctr + x"01";  

            elsif (ctr < x"21") then -- Waiting for ALU to div

              ctr      <= ctr + x"01";  

            elsif (ctr = x"21") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr      <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"22") then -- Writing back to Registers

              ctr       <= (others => '0');

            end if;
          
          when x"5" => -- AND Instruction
          
            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_num_2 <= IM_out(11 downto 8);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing AND in ALU

              a_in     <= Reg_out_1;
              b_in     <= Reg_out_2;
              alu_ctrl <= x"4";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU to AND

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Writing back to Registers

              ctr       <= (others => '0');

            end if;

          when x"6" => -- ORR Instruction
          
            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_num_2 <= IM_out(11 downto 8);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing ORR in ALU

              a_in     <= Reg_out_1;
              b_in     <= Reg_out_2;
              alu_ctrl <= x"5";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU to ORR

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Writing back to Registers

              ctr       <= (others => '0');

            end if;
          
          when x"7" =>  -- Branch Less Than 

            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(3 downto 0);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Setting up ALU for less than compare

              a_in     <= Reg_out_1;
              b_in     <= (others => '0');
              alu_ctrl <= x"9";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU to compare

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Checking the true flag for jump

              if (true_flag = '1') then 
     
                PC <= PC + (x"02" * IM_out(11 downto 4)); -- The condition is true so jump

              else 
     
                PC <= PC + x"0002"; -- The condition is false so move to next instruction

              end if;

              ctr       <= ctr + x"01"; 

            elsif (ctr = x"05") then -- Do nothing

              ctr       <= (others => '0');

            end if;
                              
          when x"8" =>  -- Branch Greater Than 

            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(3 downto 0);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Registers to read out values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Setting up ALU for greater than compare

              a_in     <= Reg_out_1;
              b_in     <= (others => '0');
              alu_ctrl <= x"A";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU to compare

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Checking the true flag for jump

              if (true_flag = '1') then 
     
                PC <= PC + (x"02" * IM_out(11 downto 4)); -- The condition is true so jump

              else 
     
                PC <= PC + x"0002"; -- The condition is false so move to next instruction

              end if;

              ctr       <= ctr + x"01"; 

            elsif (ctr = x"05") then -- Do nothing

              ctr       <= (others => '0');

            end if;

          when x"9" => -- ADDI Instruction
          
            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Register values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing Add in ALU

              a_in     <= Reg_out_1;
              b_in     <= x"000" & IM_out(11 downto 8); -- Constant value
              alu_ctrl <= x"0";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU Result

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Do nothing

              ctr       <= (others => '0');

            end if;

          when x"A" => -- SUBI Instruction
          
            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Register values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing Sub in ALU

              a_in     <= Reg_out_1;
              b_in     <= x"000" & IM_out(11 downto 8); -- Constant value
              alu_ctrl <= x"1";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU Result

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Do nothing

              ctr       <= (others => '0');

            end if;

          when x"B" => -- LSL Instruction
          
            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Register values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing LSL in ALU

              a_in     <= Reg_out_1;
              b_in     <= x"000" & IM_out(11 downto 8); -- Constant value
              alu_ctrl <= x"6";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU Result

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Do nothing

              ctr       <= (others => '0');

            end if;

          when x"C" => -- LSR Instruction
          
            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for Register values

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing LSR in ALU

              a_in     <= Reg_out_1;
              b_in     <= x"000" & IM_out(11 downto 8); -- Constant value
              alu_ctrl <= x"7";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU Result

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Writing back to register

              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= Alu_out;
              Reg_Wr    <= '1';
              ctr       <= ctr + x"01"; 
              PC        <= PC + x"0002"; 

            elsif (ctr = x"05") then -- Do nothing

              ctr       <= (others => '0');

            end if;

          when x"D" => -- LDR Instruction

            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for DM out

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing Add in ALU

              a_in     <= Reg_out_1;
              b_in     <= x"000" & IM_out(11 downto 8);
              alu_ctrl <= x"0";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU output

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Setting up DM

              DM_Rden  <= '1';
              DM_Addr  <= Alu_out;
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"05") then -- Waiting for data from DM

              ctr      <= ctr + x"01"; 
              PC       <= PC + x"0002"; 

            elsif (ctr = x"06") then -- Writing back to Registers
          
              Reg_wr    <= '1';
              Reg_num_1 <= IM_out(3 downto 0);
              Reg_in    <= DM_out;
              ctr       <= x"00";

            end if;

          when x"E" => -- STR Instruction

            if (ctr = x"0") then  -- Reading out from Registers
 
              Reg_num_1 <= IM_out(7 downto 4);
              Reg_num_2 <= IM_out(3 downto 0);
              Reg_Wr    <= '0';          
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Waiting for DM out

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"02") then -- Performing Add in ALU

              a_in     <= Reg_out_1;
              b_in     <= x"000" & IM_out(11 downto 8);
              alu_ctrl <= x"0";
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"03") then -- Waiting for ALU output

              ctr      <= ctr + x"01"; 

            elsif (ctr = x"04") then -- Setting up DM

              DM_Rden  <= '0';
              DM_Wren  <= '1';
              DM_Addr  <= Alu_out;
              DM_in    <= reg_out_2;
              ctr      <= ctr + x"01"; 

            elsif (ctr = x"05") then -- Waiting for data from DM

              ctr      <= ctr + x"01"; 
              PC       <= PC + x"0002"; 

            elsif (ctr = x"06") then -- Writing back to Registers
          
              ctr       <= x"00";

            end if;

          when x"F" =>  -- Branch 

            if (ctr = x"00") then
      
              PC <= PC + (x"2" * IM_out(11 downto 0)); -- jump
              ctr       <= ctr + x"01"; 

            elsif (ctr = x"01") then -- Do nothing

              ctr       <= (others => '0');

            end if;          

          when others => -- Do nothing

        end case;

      end if;
    
     end if;

    end if;

  end process;

  IM_wren <= Ins_wren when start = '0' else '0';
  IM_Addr <= Ins_Addr when start = '0' else PC;
  IM_in   <= Ins_in when start = '0' else (others => '0');


  IM: ins_mem
     GENERIC MAP (
                   DATA_WIDTH    => 16,
                   ADDRESS_WIDTH => 16
     )

     PORT MAP(
                  Clock       => Clock,
                  Reset       => Reset,
                  DataIn      => IM_in,
                  Address     => IM_Addr,
                  WriteEn     => IM_wren,
                  Enable      => IM_En,
                  DataOut     => IM_out
     );
     
  REG: regs
     GENERIC MAP (
                   DATA_WIDTH    => 16,
                   REGS_NUM      => 4
     )

     PORT MAP(
                  Clock       => Clock,
                  Reset       => Reset,
                  DataIn      => Reg_in,
                  Reg_num_1   => Reg_num_1,
                  Reg_num_2   => Reg_num_2,
                  RegWrite    => Reg_Wr,
                  Enable      => Reg_En,
                  DataOut_1   => Reg_out_1,
                  DataOut_2   => Reg_out_2
     );
     
  ARITH: alu
     GENERIC MAP (
                   DATA_WIDTH    => 16
     )

     PORT MAP(
                  Clock       => Clock,
                  Reset       => Reset,
                  a_in        => a_in,
                  b_in        => b_in,
                  alu_ctrl    => Alu_ctrl,
                  Enable      => Alu_En,
                  true_flag   => true_flag,
                  Res_out     => Alu_out
     );
     
  DM: data_mem
     GENERIC MAP (
                   DATA_WIDTH    => 16,
                   ADDRESS_WIDTH => 16
     )

     PORT MAP(
                  Clock       => Clock,
                  Reset       => Reset,
                  DataIn      => DM_in,
                  Address     => DM_Addr,
                  MemWrite    => DM_Wren,
                  MemRead     => DM_Rden,
                  Enable      => DM_En,
                  DataOut     => DM_out
     );
     

End Architecture;



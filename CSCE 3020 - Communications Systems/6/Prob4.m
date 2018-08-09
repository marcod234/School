%part i

num = [0 1 2]; % s + 2
den = [1 11 30]; %S^2 + 11s + 30
[r,p] = residue(num,den)

%part2

num = [0 0 0 1 2];% s + 2
den = [1 11 30 0 0];%s^4 + 11s^3 + 30s^2
[r2,p2] = residue(num,den)
# RM Least Upper Bound

$S_i$ = Service $1$ to $m$

$C_i$ = Capacity for each service

$T_i$ = Period or the if $\frac{1}{T}=f$ frequency

$$ \large U = \sum_{i=m}^{m} (C_i/T_i) \leq m(2^{\frac{1}{m}}-1) $$

This means the utilization of any processor running multiple services 1 to $m$  is the capacity (computational requirements) for that service divided by its period $C_i/T_i$ and that $U$ (utilization) should be less than a bound $m(2^{\frac{1}{m}}-1)$. This is just a mathematical property of $2^{\frac{1}{m}}-1$ times $m$ which at the end menas as uou have more and more services that share  a core, shouldn't use more than the 70% of the computational capability of that core.

![image](https://user-images.githubusercontent.com/32500615/252506054-10e22e2d-a2d4-496d-8de1-55aa3556c333.png)
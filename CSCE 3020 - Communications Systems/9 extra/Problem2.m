clc; clear all close all;

w = logspace(0,5);

c1 = 1e-9; c2 = 10e-6; r2 = 47e3;
x = -(c1*r2*1i.*w)./((c2*r2*1i.*w)+1);

 
subplot(2,1,1) ; p = semilogx(w,20*log10(abs(x)),'k') ;
set(p,'LineWidth',2) ; grid on ;
xlabel('\omega','FontSize',18,'FontName','Times') ;
ylabel('|x({\itj}\omega)|_d_B','FontSize',18,'FontName','Times') ;
title('Magnitude','FontSize',24,'FontName','Times') ;
set(gca,'FontSize',14,'FontName','Times') ;

subplot(2,1,2) ; p = semilogx(w,angle(x),'k') ;
set(p,'LineWidth',2) ; grid on ;
xlabel('\omega','FontSize',18,'FontName','Times') ;
ylabel('Phase of x{\itj}\omega)','FontSize',18,'FontName','Times') ;
title('Phase','FontSize',24,'FontName','Times') ;
set(gca,'FontSize',14,'FontName','Times') ;



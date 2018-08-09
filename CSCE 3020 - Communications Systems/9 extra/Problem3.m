clc; clear all; close all;

w = -2*pi:.001:2*pi;

x = (exp(1i.*w) - .5)./((exp(1i.*w) + .75).*(exp(1i.*w) + (5/6)).*(exp(1i.*w) + (1/6)));

subplot(2,1,1) ; p = plot(w,20*log10(abs(x)),'k') ;
set(p,'LineWidth',2) ; grid on ;
xlabel('\omega','FontSize',18,'FontName','Times') ;
ylabel('|x({\itj}\omega)|_d_B','FontSize',18,'FontName','Times') ;
title('Magnitude','FontSize',24,'FontName','Times') ;
set(gca,'FontSize',14,'FontName','Times') ;

subplot(2,1,2) ; p = plot(w,angle(x),'k') ;
set(p,'LineWidth',2) ; grid on ;
xlabel('\omega','FontSize',18,'FontName','Times') ;
ylabel('Phase of x{\itj}\omega)','FontSize',18,'FontName','Times') ;
title('Phase','FontSize',24,'FontName','Times') ;
set(gca,'FontSize',14,'FontName','Times') ;




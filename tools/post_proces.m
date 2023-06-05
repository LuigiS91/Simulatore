clear all
close all
clc

stati=struct();

tmp=dlmread('output/stati.txt',"\t");
stati_field={"t";"u";"v";"w";"p";"q";"r";"phi";"psi";"theta";"h"};
for i_stati=1:length(stati_field)
  stati.(stati_field{i_stati})=tmp(:,i_stati);
end

tmp=dlmread('output/comandi.txt',"\t");
com_field={"t";"de";"da";"dr";"rpm"};
for i_com=1:length(com_field)
  com.(com_field{i_com})=tmp(:,i_com);
end

clear tmp


figure('Units','normalized','OuterPosition',[0 0.05 0.5 0.45]);

subplot(5,2,1)
plot(stati.t,stati.u)
ylabel('u [m/s]')
grid minor

title('longitudinale')
subplot(5,2,2)
plot(stati.t,stati.v)
ylabel('v [m/s]')
grid minor

title('latero-direzionale')
subplot(5,2,3)
plot(stati.t,stati.w)
ylabel('w [m/s]')
grid minor

subplot(5,2,4)
plot(stati.t,stati.p*180/pi)
ylabel('p [deg/s]')
grid minor

subplot(5,2,5)
plot(stati.t,stati.q*180/pi)
ylabel('q [deg/s]')
grid minor

subplot(5,2,6)
plot(stati.t,stati.r*180/pi)
ylabel('r [deg/s]')
grid minor

subplot(5,2,7)
plot(stati.t,stati.theta*180/pi)
ylabel('theta [deg]')
grid minor

subplot(5,2,8)
plot(stati.t,stati.phi*180/pi)
ylabel('phi [deg]')
grid minor

subplot(5,2,9)
plot(stati.t,stati.h)
ylabel('h [m]')
xlabel('t [s]');
grid minor

subplot(5,2,10)
plot(stati.t,stati.psi*180/pi)
ylabel('psi [deg]')
xlabel('t [s]');
grid minor

figure('Units','normalized','OuterPosition',[0 0.5 0.25 0.5]);
subplot(4,1,1)
title('comando')
plot(com.t,com.de*180/pi);
ylabel('de [deg]')
grid minor

subplot(4,1,2)
plot(com.t,com.da*180/pi);
ylabel('da [deg]')
grid minor

subplot(4,1,3)
plot(com.t,com.dr*180/pi);
ylabel('dr [deg]')
grid minor

subplot(4,1,4)
plot(com.t,com.rpm);
ylabel('rpm [rpm]')
xlabel('t [s]');
grid minor


x=zeros(2,length(stati.t));
V=zeros(1,length(stati.t));
alpha=zeros(1,length(stati.t));
beta=zeros(1,length(stati.t));
dh=zeros(1,length(stati.t));

figure('Units','normalized','OuterPosition',[0.25 0.5 0.25 0.5]);
dt=0.01;
for i=1:length(stati.t)
    u=stati.u(i);
    v=stati.v(i);
    w=stati.w(i);
    theta=stati.theta(i);
    psi=stati.psi(i);
    phi=stati.phi(i);
    V(i)=sqrt(u^2+v^2+w^2);
    alpha(i)=atan2(w,u);
    beta(i)=atan2(v,V(i));
    x(1,i+1)=x(1,i)+(u*cos(theta)*cos(psi)+...
    v*(sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi))+...
    w*(cos(phi)*sin(theta)*cos(psi)+sin(phi)*sin(psi)))*dt;
    x(2,i+1)=x(2,i)-((u*cos(theta)*sin(psi)+...
    v*(cos(phi)*cos(psi)+sin(phi)*sin(theta)*sin(psi))+...
    w*(cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi))))*dt;
    dh(i)=u*sin(theta)-v*sin(phi)*cos(theta)-w*cos(phi)*cos(theta);
end

plot3(x(1,1:end-1),x(2,1:end-1),stati.h,x(1,1),x(2,1),stati.h(1),'ro',x(1,end),x(2,end),stati.h(end),'bo');
limite=max(max(abs(x)));
axis([-limite limite -limite limite 0 max(stati.h)])
xlabel('x [m]');
ylabel('y [m]');
zlabel('h [m]');
grid minor
legend('traiettoria','partenza','arrivo','Location','best')

figure('Units','normalized','OuterPosition',[0.5 0.5 0.25 0.5]);
plot(x(1,1:end-1),x(2,1:end-1),x(1,1),x(2,1),'ro',x(1,end),x(2,end),'bo');
axis([-limite limite -limite limite])
xlabel('x [m]');
ylabel('y [m]');
grid minor;
legend('traiettoria','partenza','arrivo','Location','best')

figure('Units','normalized','OuterPosition',[0.75 0.5 0.25 0.5]);
plot(x(1,1:end-1),stati.h,x(1,1),stati.h(1),'ro',x(1,end-1),stati.h(end),'bo');
axis([-limite limite 0 5000])
xlabel('x [m]');
ylabel('h [m]');
grid minor;
legend('traiettoria','partenza','arrivo','Location','best')

figure('Units','normalized','OuterPosition',[0.75 0.05 0.25 0.45]);
plot(x(2,1:end-1),stati.h,x(2,1),stati.h(1),'ro',x(2,end-1),stati.h(end),'bo');
axis([-limite limite 0 5000])
xlabel('y [m]');
ylabel('h [m]');
grid minor;
legend('traiettoria','partenza','arrivo','Location','best')

figure('Units','normalized','OuterPosition',[0.5 0.05 0.25 0.45]);
subplot(4,1,1)
plot(stati.t,V);
ylabel('V [m/s]');
grid minor

subplot(4,1,2)
plot(stati.t,alpha*180/pi);
ylabel('alpha [deg]');
grid minor

subplot(4,1,3)
plot(stati.t,beta*180/pi);
ylabel('beta [deg]');
grid minor;

subplot(4,1,4)
plot(stati.t,dh);
ylabel('dh [m/s]');
xlabel('t [s]');
grid minor;

type output/warning_errori.txt

pause()

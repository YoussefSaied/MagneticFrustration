set(groot, 'defaultAxesTickLabelInterpreter','latex');
set(groot, 'defaultLegendInterpreter','latex');
set(groot, 'defaultTextInterpreter', 'latex');
set(groot, 'defaultAxesFontSize', 18);


%% Chargement des resultats %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fichier = 'output';
data = load([fichier,'_obs.out']);
t = data(:,1);
E = data(:,2);
data = load([fichier,'_pot.out']);
x = data(:,1);
V = data(:,2);
psi2 = load([fichier,'_psi2.out']);

%% Figures %%
%%%%%%%%%%%%%
%figure('Name',['Analyse de ' fichier])



figure
plot(x,V)
xlabel('Position x [m]')
ylabel('Potential V(x) [J]')

figure
plot(t,E)
xlabel('Time t [s]')
ylabel('Energy H [J]')

etalement_exact = dxmoy(1) .* sqrt(1 + ((t.^2) / (256.0*0.1)^4) );
figure
plot(t,dxmoy)
grid on
hold on
plot(t,etalement_exact,'--')
xlabel('Time t [s]')
ylabel('$<\Delta x> [m]$')
legend('$<\Delta x>$ numerique','$<\Delta x>$ analytique')

figure
plot(t,dpmoy)
grid on
xlabel('Time t [s]')
ylabel('Dp')

figure
plot(t,P1,t,P2,t,P3,t,P1+P2+P3)
xlabel('Time t [s]')
ylabel('Probability [ ]')
legend('$P_{x<x_a}$','$P_{x_a \le x \le x_b}$','$P_{x>x_b}$','$P_{tot}$')

%%
figure
[X,T] = meshgrid(x,t);
pcolor(X,T,psi2)
xlabel('Position x [m]')
ylabel('Time t [s]')
shading interp
colorbar
%%
%{
figure
plot(t,xmoy,t,x2moy,t,pmoy,t,p2moy)
xlabel('Time t [s]')
legend('$<x>$','$<x^2>$','$<p>$','$<p^2>$')
%}

figure
plot(t,xmoy-128)
xlabel('Time t [s]')
%ylabel('$<x>$ [m]')
ylabel('Position [m]')
hold on
plot(t,34.89607686*sin(0.02236068.*t))
legend('$<x>$ quantum particle','$x$ quantum particle')


figure
plot(t,pmoy)
xlabel('Time t [s]')
%ylabel('$<p>$ [m]')
ylabel('Momentum p')
hold on
plot(t,34.89607686*0.02236068*cos(0.02236068.*t))
legend('quantum particle: $<p>$ ','classical particle: $p$ ')


figure
plot(t,dxmoy.*dpmoy)
grid on
hold on
plot(t,dxmoy.*0 + 0.5)
xlabel('Time t [s]')
ylabel('Uncertainty')
legend('$<\Delta x> <\Delta p>$','$y = \frac{1}{2}$')

%%

figure
h(1) = plot(x,sqrt(psi2(1,:)));
%A=arrayfun(@hf,x);
hold on
%h(2)= copyobj(h(1),gca);
% fplot(hf,[min(f(:)),max(f(:))])
% hold off
grid
xlabel('Position x [m]')
ylabel('$| \psi(x,t) | ^2$')
ht = title('t=0 s');
%ylim([min(f(:)),max(f(:))])
bo=1;
for i=2:length(t)
    %if bo == 1
       % pause;
        %bo=0;
    %end
    pause(0.1)
    if ~ishandle(h)
        break % Arrete l'animation si la fenetre est fermee
    end
    set(h(1),'YData',sqrt(psi2(i,:)))
    %set(h(2),'YData',A)
    set(ht,'String',sprintf('t=%0.2f s',t(i)))
end

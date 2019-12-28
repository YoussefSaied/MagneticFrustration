%%Text format%%
set(groot, 'defaultAxesTickLabelInterpreter','latex');
set(groot, 'defaultLegendInterpreter','latex');
set(groot, 'defaultTextInterpreter', 'latex');
set(groot, 'defaultAxesFontSize', 18);

%%Analysis%%

%%Unpack Data%%
nsimul = 1000;
%variable glossary:
%E = energy
%C = correlation
%A = total angle
%T = type
  %m_T = magnet type = ff,fw,ww
  %v_T = magnet types on a vertex (x3)
  %d_T = magnet types on a double vertex (x5)
  %f_T = magnet types on a face (x5)
%O = face orientation = 5,4,3,2
evolve = 'evolve';
load('data/evolve/angles'); %nx(T,E,C,ax30)
load('data/evolve/dodecs'); %n*dodectype
load('data/evolve/faces');
load('data/evolve/vertices'); %nx20x(energy, magnetindex1,2,3)
load('data/evolve/magnets'); %nx30x(a,T)

%% Youssef Vertex type (Important):

% Simplify magnets type (2 ,3 -> 2):

magnetsSimplified = magnets;

for i =1:nsimul
    for j=1:30
        type =floor(mod(magnetsSimplified(i,j,2),1000)/100);
        if type == 2 || type == 3
            type =2;
        end
        magnetsSimplified(i,j,2) = type;
    end
end

% Vertex with simple Dvertex types:

verticesDV =zeros(nsimul,20,2);

for i =1:nsimul
    for j=1:20
        verticesDV(i,j,1) = vertices(i,j,1);
        tempMagnetTypes = sort([magnetsSimplified(i,1+vertices(i,j,2),2) magnetsSimplified(i,1+vertices(i,j,3),2) magnetsSimplified(i,1+vertices(i,j,4),2)]);
%          for l = 1:3
%              if tempMagnetTypes(3) == 4
%          end
        if tempMagnetTypes(3) == 4
            for l = 1:2
                if tempMagnetTypes(l) == 3
                    tempMagnetTypes(l)=2;
                end
            end
        end
        if tempMagnetTypes(3) == 3
            for l = 1:2
                if tempMagnetTypes(l) == 3
                    tempMagnetTypes(l)=2;
                end
            end
        end
        if tempMagnetTypes(3) == 2
            tempMagnetTypes(3)=3;
        end
        verticesDV(i,j,2) = tempMagnetTypes(3)*100 + tempMagnetTypes(2)*10 + tempMagnetTypes(1)  ;
    end
end
UniqueVerticesDV = unique(verticesDV(:,:,2));
% VerticesDV and Energy:
%verticesDV1 = reshape(verticesDV, [nsimul*20,2]);
verticesDV1 = zeros(nsimul*20,2);
for i =1:nsimul
    for l=1:20
        verticesDV1((i-1)*20 +l,1) = verticesDV(i,l,1);
        verticesDV1((i-1)*20 +l,2) = verticesDV(i,l,2);
    end
end
x= verticesDV1;
C = unique(x(:,2));
A = zeros(length(x),length(C));
A = A*nan;
for i = 1:length(x)
    for j = 1:length(C)
        if x(i,2) == C(j)
            A(i,j) = x(i,1);
        end
    end
end
A(A==0) = nan;


figure
xbins = [-300: -160]/2 *10^-7;
hist(A(:,1:length(C)),xbins);
colormap jet
grid on
xlabel('Energy [J]');
ylabel('Count')
title('Vertex Energy');
legend(num2str(C(1:length(C))));

% VerticesDV Histogram:
figure

hist(verticesDV1(:,2))

colormap jet
grid on
xlabel('Vertex Types');
ylabel('Count')
title('Vertex Types');
%legend(num2str(C(1:length(C))));

AverageVertexEnergy = zeros(nsimul,1);
for i= 1:nsimul
    for l =1:20
        AverageVertexEnergy(i) = AverageVertexEnergy(i) + vertices(i,l,1);
    end
end
AverageVertexEnergy;
%% Youssef: Vertex Dodec (Important): 
Dodecvertex = zeros(nsimul,1);

for i = 1:length(Dodecvertex)
    for j = 1:length(UniqueVerticesDV)
        for l = 1:20
            if UniqueVerticesDV(j) == verticesDV(i,l,2)
               Dodecvertex(i) = Dodecvertex(i)+ 10^(j-1); 
            end
        end
    end
end

UniqueDodecvertex = unique(Dodecvertex);

% Energy- Dodectypevertex 
figure
C = UniqueDodecvertex;
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if Dodecvertex(i) == C(j)
            A(i,j) = angles(i,2);
        end
    end
end


range = [4,5,10];
%range = 1:length(C);
hist(A(:,range),50);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Energy - Dodec configuration');
legend(num2str(C(range)));

% FF - Dodectypevertex 
figure
C = UniqueDodecvertex;
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if Dodecvertex(i) == C(j)
           tmp =0;
           for l = 1:12
                tmp = tmp + faces(i,l,8);
           end
           tmp = tmp/2; %this is #FF
           A(i,j) = tmp;
        end
    end
end


range = [4,5,10];
%range = 1:length(C);
hist(A(:,range),50);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('FF - Dodec configuration');
legend(num2str(C(range)));


%% system
load('data/evolve/angles'); %nx(T,E,C,ax30)
s = angles;
s_a = s(:,5:34);
for i = 1:nsimul
    for j = 1:30
        s_a(i,j) = mod(s_a(i,j),pi);
    if s_a(i,j)>pi/2
        q = s_a(i,j);
        s_a(i,j) = pi - q;
        s_a(i,j) = mod(s_a(i,j),pi);
    end
    end
end
s_Aabs = sum(abs(s_a),2);
s_A = sum(s_a,2);
s_E = s(:,2);
s_C = s(:,4); %maybe need a couple of different correlations


figure %histograms of energy, correlation & angle
subplot(1,3,1)
histogram(s_E,10);
% h = findobj(gca,'Type','patch');
% 
%  h.FaceColor = [0 0.5 0.5];
%  h.EdgeColor = 'w';
grid on
title('E');
subplot(1,3,2)
hist(s_C,10);
% h = findobj(gca,'Type','patch');
% 
%  h.FaceColor = [0 0.5 0.5];
%  h.EdgeColor = 'w';
grid on
title('C');
subplot(1,3,3)
grid on
%histogram(mod(s_Aabs,pi),100)
hist(s_Aabs,30,'Normalization','pdf')
% h = findobj(gca,'Type','patch');
% 
%  h.FaceColor = [0 0.5 0.5];
%  h.EdgeColor = 'w';
hold on
x = 6:.1:13;
norm = normpdf(x,mean(s_A),std(s_A));
plot(x,norm);
title('A')
%% histograms of energy, correlation & angle 
figure %histograms of energy, correlation & angle
subplot(1,2,1)
histogram(s_E,10,'FaceColor',[0 0.5 0.5] );


 %h.FaceColor = [0 0.5 0.5];
 %h.EdgeColor = 'w';
grid on
xlabel('Energy [J] $10^{-5}$');
subplot(1,2,2)
histogram(s_C,10,'FaceColor',[0 0.5 0.5] );

% h.EdgeColor = 'w';
grid on
xlabel('Correlation []');

%%
figure
grid on
%histogram(mod(s_Aabs,pi),100)
histogram(s_Aabs,30,'Normalization','pdf','Facecolor',[0.25, 0.25, 0.25])

hold on
x = 6:.1:13;
norm = normpdf(x,mean(s_A),std(s_A));
plot(x,norm,'r--');
title('Sum of angles (pdf)')


figure %scatter E vs C, E vs A
subplot(1,2,1)
hold on
scatter(s_C,s_E,10,'MarkerEdgeColor',[0 .5 .5],'MarkerFaceColor',[0 .7 .7],'LineWidth',0.1);
R = corrcoef(s_C,s_E);
grid on
xlabel('cor');
ylabel('E');
subplot(1,2,2)
hold on
scatter(s_Aabs,s_E,10,'MarkerEdgeColor',[0 .5 .5],'MarkerFaceColor',[0 .7 .7],'LineWidth',0.1);
grid on
xlabel('angle')
ylabel('E')

%face angle sum:
Sumofanglesonaface = zeros(nsimul,12);
for i= 1:nsimul
    for j=1:12
        for k=1:5
        Sumofanglesonaface(i,j) = Sumofanglesonaface(i,j) + s_a(i,faces(i,j,k)+1);
        end
    end
end



figure
histogram(Sumofanglesonaface(:,1),30,'Normalization','pdf')
hold on
x = 0:.1:10;
norm = normpdf(x,mean(Sumofanglesonaface(:,1)),std(Sumofanglesonaface(1,:)));
plot(x,norm);
title("Face angle sum")

%sum of a closed loop:

Sumofanglesonaclosedloop = zeros(nsimul,1);
for i= 1:nsimul
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,6);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,7);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,9);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,10);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,12);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,13);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,25);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,23);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,28);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,27);
end



figure
hist(Sumofanglesonaclosedloop)
title("Grandloop-angles sum")


%all magnets
figure
hist(reshape(s_a,[nsimul*30,1]),100)
title("one magnet")

%sum of random magnets:

Sumofanglesonaclosedloop = zeros(nsimul,1);
for i= 1:nsimul
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,1);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,6);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,21);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,24);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,15);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,13);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,29);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,9);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,17);
        Sumofanglesonaclosedloop(i) = Sumofanglesonaclosedloop(i) + s_a(i,4);
end
figure
hist(Sumofanglesonaclosedloop)
title("Sum of angles on 12 random magnets")

%even better:
figure
hist(mean(s_a(:,1:30))*30,10)


%all magnets
figure
hist(s_a(1,:),100)
title("1 angle")

%30 random magnets
randommagnet =randi([1 30],1,30*nsimul);
randomsimulation =randi([1 nsimul],1,nsimul);
sumof30angles= zeros(1,nsimul);
for i =1:nsimul
    for j = 1:30
      sumof30angles(i)= sumof30angles(i)+s_a(randomsimulation(i),randommagnet(30*(i-1)+j));  
    end
end


figure
histogram(sumof30angles,30,'Normalization','pdf')
hold on
x = 0:.1:20;
norm = normpdf(x,mean(sumof30angles),std(sumof30angles));
plot(x,norm);
title("sumof30angles")
std(sumof30angles)
%sumof 0 and 1 and 26 4 14 +s_a(:,5) +s_a(:,15)

sum01= s_a(:,30)+s_a(:,29) +s_a(:,28);
sum02= s_a(:,9)+s_a(:,10) +s_a(:,8);
sum10 =sum01+sum02;
figure
subplot(1,3,1)
hist(sum02,100)
title("sumofanglesatavertex")
subplot(1,3,2)
hist(sum01,100)
title("sumofanglesatavertex2")
subplot(1,3,3)
hist(sum10/2,100)
title("sum of angles at opposite vertices")



%% Youssef: Magnets (Important)

load('data/evolve/magnets'); %nx30x(a,T)
load('data/evolve/doublevertices');
%m = reshape(magnets,[nsimul,30,2]);
m= magnets;
m_A = m(:,:,1);
m_T = m(:,:,2);
for i = 1:nsimul
    for j = 1:30
        m_A(i,j) = mod(m_A(i,j),pi);
    if m_A(i,j)>pi/2
        q = m_A(i,j);
        m_A(i,j) = pi - q;
        m_A(i,j) = mod(m_A(i,j),pi);
    end
    end
end


%Energy-Dvertex basic
m_Tbasic = floor(mod(m_T,1000)/100);
m_Tbasic1 =zeros(30*nsimul,1);
for i =1:nsimul
    for j =1:30
        m_Tbasic1((i-1)*30 + j) = m_Tbasic(i,j);
    end
end
m_Tbasic1(m_Tbasic1 == 2) =3;
C = unique(m_Tbasic1);
A = zeros(length(m_Tbasic1),length(C));
%A = A*nan;

for i = 1:length(m_Tbasic1)
    for j = 1:length(C)
        if m_Tbasic1(i) == C(j)
            firstindex= (i -(mod(i-1,30)+1))/30 +1;
            for l = 1:30
                secondindex =mod(i-1,30);
                if secondindex == doublevertices(firstindex,l,1)
                A(i,j) = doublevertices(firstindex,l,2);
                end
            end
        end
    end
end
h= figure
colormap(h,jet) 
%xbins = [-540: -300]/18*10^-6;
tmp = A(:,1);
tmp(tmp==0)= [];
histogram(tmp,100,'Normalization','pdf');
hold on
tmp = A(:,2);
tmp(tmp==0)= [];
histogram(tmp,100,'Normalization','pdf');
tmp = A(:,3);
tmp(tmp==0)= [];
histogram(tmp,100,'Normalization','pdf');
grid on
xlabel('Energy [J]');
ylabel('Count')
legend('WW','FW','FF');




%FF energy sum
EnergysumFF=zeros(nsimul,1);
for i = 1:nsimul
    for j =1:30
        EnergysumFF(i) = EnergysumFF(i) + A((i-1)*30+j,2);
    end
end
figure
hist(EnergysumFF)

% Important : Angle-Dvertex basic

m_Tbasic = floor(mod(m_T,1000)/100);
m_Tbasic1 =zeros(30*nsimul,1);
for i =1:nsimul
    for j =1:30
        m_Tbasic1((i-1)*30 + j) = m_Tbasic(i,j);
    end
end
m_Tbasic1(m_Tbasic1 == 2) =3;
C = unique(m_Tbasic1);
A = zeros(length(m_Tbasic1),length(C));
A = A*nan;

for i = 1:length(m_Tbasic1)
    for j = 1:length(C)
        if m_Tbasic1(i) == C(j)
            firstindex= (i -(mod(i-1,30)+1))/30 +1;
            A(i,j) = m_A(firstindex,mod(i-1,30)+1);
        end
    end
end
figure
xbins = [0:150]/100;
colormap(jet) 
histogram(A(:,1),xbins,'Normalization','pdf')
hold on
histogram(A(:,2),xbins,'Normalization','pdf')
hold on
histogram(A(:,3),xbins,'Normalization','pdf')
grid on
xlabel('Angle [rad]');
ylabel('Count')
legend('WW','FW','FF');



%% Youssef: DVertex detailled:
m_ff_A = zeros(nsimul*30,3); %11411, 12411, 12421, (angle,type,energy)
m_fw_A = zeros(nsimul*30,3);
m_ww_A = zeros(nsimul*30,3);
m_wwsimp_A = zeros(nsimul*30,3);
m_wwsimp2_A = zeros(nsimul*30,3);
m_fff_A = zeros(size(m_A));

for i = 1:nsimul
  for j = 1:30
      q = m_T(i,j);
      m5 = int32(q/10000);
      m4 = int32(mod(q,10000)/1000);
      m3 = int32(mod(q,1000)/100);
      m2 = int32(mod(q,100)/10);
      m1 = int32(mod(q,10));
      ms = [m1,m2,m3,m4,m5];

    if m3 == 4 %strong
        m_ff_A((i-1)*30+j,1) = m_A(i,j);
        m_ff_A((i-1)*30+j,2)= q ;
        %m_ff_A((i-1)*30+j,3) =doublevertices(i,j,2);
    if m1 == m5 && m2 == m4
        m_ff_A((i-1)*30+j,2) = min(m1,m2) + max(m1,m2)*10 + 400 + max(m5,m4)*1000 + min(m5,m4)*10000;   
    end 
    if m1 == m4 && m2 == m5
        m_ff_A((i-1)*30+j,2) = max(m1,m2) + min(m1,m2)*10 + 400 + max(m5,m4)*1000 + min(m5,m4)*10000;   
    end
    if m1==m2 
        tmp= min([m4,m5]) + 10*max([m4,m5])+ 400+ (m2*10+m1)*1000;
        m_ff_A((i-1)*30+j,2) =  tmp;
    end
    if m4==m5
        tmp= min([m1,m2]) + 10*max([m1,m2])+ 400+ (m5*10+m4)*1000;
        m_ff_A((i-1)*30+j,2) =  tmp;
    end
    if m4==m5 && m1==m2
        tmp= min(m1+m2*10,m4 +m5*10)+ 400 + max(m1+m2*10,m4 +m5*10)*1000  ;
        m_ff_A((i-1)*30+j,2) =  tmp;
    end
    


    
    
    elseif m3 == 2 || m3 == 2 
        m_fw_A((i-1)*30+j,1) = m_A(i,j);
        m_fw_A((i-1)*30+j,2) = q;
        %m_fw_A((i-1)*30+j,3) =doublevertices(i,j,2);
        if m3 == 2
            m3 = 3;
            tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
            m1 = tmpm4; m2 = tmpm5; m4 = tmpm1; m5 = tmpm2;
        end
        if m4==m5 %horizontal reflection
           tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
           m1 = max(tmpm1,tmpm2); m2 =min(tmpm1,tmpm2);
        end
       

        if m5>m4
            tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
            m1 = tmpm2; m2 = tmpm1; m4 = tmpm5; m5 = tmpm4;
        end
        m_fw_A((i-1)*30+j,2) = m1 + m2*10 + m3*100 + m4*1000 + m5*10000;
        %m_fwsimp_A((i-1)*30+j,2) = max(m1,m2) + min(m1,m2)*10 + m3*100 + m4*1000 + m5*10000;
     
    elseif m3 == 1 %weak
      ms(ms==3)=2;
      %q = 10000*ms(1)+ 1000*ms(2)+100*ms(3)+10*ms(4)+ms(5);
      m_ww_A((i-1)*30+j,1) = m_A(i,j);
      m_wwsimp_A((i-1)*30+j,1) = m_A(i,j);
      m_wwsimp2_A((i-1)*30+j,1) = m_A(i,j);
      m_ww_A((i-1)*30+j,2) =q;
      
    if m1 == m5 && m2 == m4 %180 rotation
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
         m1 =min(tmpm1,tmpm2); m2 =max(tmpm1,tmpm2); m4= max(tmpm5,tmpm4); m5= min(tmpm5,tmpm4);
    end
    if m1==m4 && m2==m5 %vertical reflection
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
        m1 = max(tmpm1,tmpm2); m4 = max(tmpm1,tmpm2);  m5=min(tmpm1,tmpm2); m2 = min(tmpm1,tmpm2);
    end
    if m1 == m4  %vertical reflection
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
        m2=max(tmpm2,tmpm5); m5=min(tmpm2,tmpm5);
    end
    if m2 == m5 %vertical reflection
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
        m1 = max(tmpm1,tmpm4); m4 = min(tmpm1,tmpm4);
    end
    if m4==m5 && m1==m2 %horizontal reflection
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
         m1 = max(tmpm1,tmpm5); m2=m1; m4 = min(tmpm1,tmpm5); m5 =m4;
    end
    if m1==m2 %horizontal reflection
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
        m4 = max(tmpm4,tmpm5); m5 =min(tmpm4,tmpm5);
    end
    if m4==m5 %horizontal reflection
       tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
       m1 = max(tmpm1,tmpm2); m2 =min(tmpm1,tmpm2);
    end
    if m1==m5 %diagonal reflection
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
        m4=min(tmpm2,tmpm4); m2=max(tmpm2,tmpm4);
    end
    if m2==m4 %diagonal reflection
        tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
        m5=min(tmpm1,tmpm5); m1=max(tmpm5,tmpm1);
    end
    tmpm1 =m1; tmpm2 = m2; tmpm4= m4; tmpm5 =m5;
    minimamldigit= min([tmpm1,tmpm2,tmpm4,tmpm5]);

    if tmpm1 == minimamldigit
        m5 = tmpm1; m4 = tmpm2; m2 = tmpm4; m1 =tmpm5;
    elseif tmpm2 == minimamldigit
        m5 = tmpm2; m4 = tmpm1; m2 = tmpm5; m1 =tmpm4;
    elseif tmpm4 == minimamldigit
        m5 = tmpm4; m4 = tmpm5; m2 = tmpm1; m1 =tmpm2;
    end  
    m_ww_A((i-1)*30+j,2) = m1 + m2*10 + m3*100 + m4*1000 + m5*10000;
    m_wwsimp_A((i-1)*30+j,2) = max(m1,m2) + min(m1,m2)*10 + m3*100 + m4*1000 + m5*10000;
    ms = [m1,m2,m5,m4];
    numFFFW=0;
    for k=1:4
        if ms(k) ~=1
            numFFFW= numFFFW+1;
        end
    end
    m_wwsimp2_A((i-1)*30+j,2) = numFFFW;
%     m_ww_A((i-1)*30+j,3) =doublevertices(i,j,2);
%     m_wwsimp_A((i-1)*30+j,3) =doublevertices(i,j,2);
%     m_wwsimp2_A((i-1)*30+j,3) =doublevertices(i,j,2);
    end  
    

    end
end

magnets30simp = zeros(30*nsimul,2);


for i=1:nsimul
    for j=1:30
      q = m_T(i,j);
      m5 = int32(q/10000);
      m4 = int32(mod(q,10000)/1000);
      m3 = int32(mod(q,1000)/100);
      m2 = int32(mod(q,100)/10);
      m1 = int32(mod(q,10));
      ms = [m1,m2,m4,m5];
      magnets30simp((i-1)*30 +j,1) = m_A(i,j);
      numFW=0;
      numFF=0;
      numWF=0;
      for l=1:4
          if ms(l)==3
              numFW= numFW +1;
          end
          if ms(l)==2
              numWF= numWF +1;
          end
          if ms(l)==4
              numFF= numFF +1;
          end
      end
      magnets30simp(j+(i-1)*30,2) = m3*1000 +numFF*100 + numFW*10 +numWF;
    end
end

x = [m_ff_A ; m_ff_A ;m_fw_A; m_fw_A; m_wwsimp2_A];

for i = 1:length(x)
    if x(i,1) == 0
        x(i,:) = 0;
    end
    x(i,1) = mod(x(i,1),pi);
    if x(i,1)>pi/2
        q = x(i,1);
        x(i,1) = pi - q;
        x(i,1) = mod(x(i,1),pi);
    end

end

C = unique(x(:,2));
A = zeros(length(x),length(C));
A = A*nan;

for i = 1:length(x)
    for j = 1:length(C)
        if x(i,2) == C(j)
            A(i,j) = x(i,1);
        end
    end
end



figure
range = 2:length(C)-0;
hold on
% for i =range
%     tmp = A(:,i);
% tmp(tmp==0)= [];
% 
% histogram(tmp,100,'FaceAlpha',0.5,'Normalization','pdf');
% end
hist(A(:,range),50);
colormap jet
grid on
xlabel('angle');
title('FF/WW/FW angles');
legend(num2str(C(range-0)));



% Dvertex detailed - Energy:

C = unique(x(:,2));
A = zeros(length(x),length(C));
A = A*nan;
count = 0;
for i = 1:length(x)
    for j = 1:length(C)
        if x(i,2) == C(j)
            firstindex= mod((i -(mod(i-1,30)+1))/30 ,1000) +1;
            for l = 1:30
                secondindex =mod(i-1,30);
                if secondindex == doublevertices(firstindex,l,1)
                A(i,j) = doublevertices(firstindex,l,2);
                end
            end
        end
    end
end
% for i = 1:length(x)
%     for j = 1:length(C)
%         if x(i,2) == C(j)
%             A(i,j) = x(i,3);
%         end
%     end
% end

figure
%xbins = [-270: -150]/9*10^-6;
%range = [2,12,7,15,14];
range = 2:length(C)-0;
hist(A(:,range),100);
hold on
% for i =range
%     tmp = A(:,i);
% tmp(tmp==0)= [];
% 
% histogram(tmp,200,'FaceAlpha',0.5,'Normalization','pdf');
% end
colormap jet
grid on
xlabel('Energy [J]');
title('FF/WW/FW Energy');
legend(num2str(C(range)));

%%
m_all_A = zeros(nsimul*30,2); %30*nsimul, [angle , type] 
m_Tbasic(m_Tbasic == 2) =3;

for i = 1:nsimul
    for j = 1:30
      q = m_T(i,j);
      m5 = int32(q/10000);
      m4 = int32(mod(q,10000)/1000);
      m3 = int32(mod(q,1000)/100);
      m2 = int32(mod(q,100)/10);
      m1 = int32(mod(q,10));
      ms1 = [m1,m2,m3,m4,m5];
      
     
      m_all_A((i-1)*30+j,1) = m_A(i,j);
      typ1=0;
      typ4=0;
      for k = 1:5
          if(k~= 3 && ms1(k) == 1) 
              typ1=typ1+1;
          end
          if(k~= 3 && ms1(k) == 4)
              typ4=typ4+1;
          end
      end
%       if m_Tbasic(i,j) == 3
%           m_all_A((i-1)*30+j,2) = m_Tbasic(i,j)*100;
%       else
      m_all_A((i-1)*30+j,2) = m_Tbasic1(30*(i-1) +j)*100 + typ1 + typ4*10;
%      end

    end
end


x = m_all_A;

for i = 1:length(x)
    if x(i,1) == 0
        x(i,:) = 0;
    end
    x(i,1) = mod(x(i,1),pi);
    if x(i,1)>pi/2
        q = x(i,1);
        x(i,1) = pi - q;
        x(i,1) = mod(x(i,1),pi);
    end

end

C = unique(x(:,2));
A = zeros(length(x),length(C));
A = A*nan;

for i = 1:length(x)
    for j = 1:length(C)
        if x(i,2) == C(j)
            A(i,j) = x(i,1);
        end
    end
end



figure
hist(A, 20);
colormap jet
grid on
xlabel('angle [rad]');
%title('Double vertex angles');
vecstrings2 = [];
vecstrings = [];
for i = 1: length(C)
if floor(C(i)/100) == 1
    vecstrings = [vecstrings; "WW" ]
end
if floor(C(i)/100) == 3
    vecstrings = [vecstrings; "FW" ]
end
if floor(C(i)/100) == 4
    vecstrings = [vecstrings; "FF" ]
end
end
for i = 1: length(C)
vecstrings2 = [vecstrings2 ; sprintf('Type %s with %d FF %d WW' ,vecstrings(i), floor(mod(C(i),100)/10),  floor(mod(C(i),10)) )];
end
legend(vecstrings2);



%% Youssef: total FF angle (important)
FFtotalangle =zeros(nsimul,1);
for i =1:nsimul
    for j =1:30
       FFtotalangle(i) =FFtotalangle(i) + m_ff_A((i-1)*30 +j,1);
    end
    
end

figure
hist(FFtotalangle)
WWtotalangle =zeros(nsimul,1);
for i =1:nsimul
    for j =1:30
       WWtotalangle(i) =WWtotalangle(i) + m_ww_A((i-1)*30 +j,1);
    end
    
end
FWtotalangle= s_A - WWtotalangle -FFtotalangle;
S1= std(WWtotalangle);
S2 = std(FFtotalangle);
R1 = corrcoef(WWtotalangle,FFtotalangle);
R2 = corrcoef(s_a(:,1),s_a(:,2));

figure
hist(WWtotalangle,50)
colormap jet
grid on
title("Total WW angle")

figure
hist(FWtotalangle,50)
title("Total FW angle")
colormap jet
grid on

figure

scatter(WWtotalangle,FWtotalangle )
title("Total WW vs Total FW angle")
colormap jet
grid on

figure

hist(WWtotalangle+FWtotalangle +0.445,50 )
hold on

title("Total WW + FW angle")
xlabel("Total WW + FW angle [rad]")
ylabel("Count")
colormap jet
grid on

figure %FW
hist(s_A - (WWtotalangle+FFtotalangle))
title("FW")
figure

scatter(s_A - (WWtotalangle+FFtotalangle),(WWtotalangle+FFtotalangle))

%energy
figure
title('Energy and FW total angle')
scatter(s_A - (WWtotalangle+FFtotalangle),s_E )

figure
title('Energy and FF total angle')
subplot(2,3,1)
grid on
scatter(FFtotalangle,s_E ,'MarkerEdgeColor',[0 .5 .5],'MarkerFaceColor',[0 .7 .7],'LineWidth',0.1);
ylabel("Energy [J] $10^{-5}$")
xlabel("FFtotal angle [rad]")
subplot(2,3,2)
grid on
scatter(WWtotalangle,s_E ,'MarkerEdgeColor',[0 .5 .5],'MarkerFaceColor',[0 .7 .7],'LineWidth',0.1);
ylabel("Energy [J] $10^{-5}$")
xlabel("WWtotal angle [rad]")
subplot(2,3,3)
grid on
scatter(s_A -FFtotalangle -WWtotalangle ,s_E ,'MarkerEdgeColor',[0 .5 .5],'MarkerFaceColor',[0 .7 .7],'LineWidth',0.1);
ylabel("Energy [J] $10^{-5}$")
xlabel("FWtotal angle [rad]")
subplot(2,3,4)
grid on
scatter(s_A ,s_E ,'MarkerEdgeColor',[0 .5 .5],'MarkerFaceColor',[0 .7 .7],'LineWidth',0.1);
ylabel("Energy [J] $10^{-5}$")
xlabel("FWtotal angle [rad]")


%% Total angle of FW and WW vs average vertex energy:

figure

AverageVertexEnergy= mean(vertices(:,:,1),2);
scatter(WWtotalangle2+FWtotalangle2, AverageVertexEnergy)
hold on

title("Total WW + FW angle vs Average Vertex Energy")
xlabel("Total WW + FW angle [rad]")
ylabel("Count")
colormap jet
grid on

%% Magnet by maintype

figure
xbins = (-160:-80)*10^-7*2;


%/length(A(:,i))
% %h.BinEdges =xbins;
% for i = 2:length(C)
% hist( A(:,i) ,xbins)/( length(A(:,i)) -sum(isnan(A(:,i))) );
% %h(i).Normalization = 'probability';
% hold on
% end
xbins = (-1600:-800)*10^-8*2;
hist(A(:,2:length(C)),50);
colormap jet
grid on
xlabel('Energy');
title('FF');
legend(num2str(C(2:length(C))));




% Energy - Vertex (Important)
v= reshape(vertices,[nsimul*20,4]);

figure('Renderer', 'painters', 'Position', [10 10 800 400])
%title('Vertices');
% xbins = (-160:-80)*10^-;

hist(v(:,1),100,'FaceColor',[0 0.5 0.5]);
%set(h,'FaceColor','r','EdgeColor','r');
%colormap jet
grid on
xlabel('Energy [J]');
ylabel('Count')
h = findobj(gca,'Type','patch');

 h.FaceColor = [0 0.5 0.5];
 h.EdgeColor = 'w';



DVert= reshape(doublevertices,[nsimul*30,3]);
figure
xbins = (-1600:-800)*10^-8*2;

hist(DVert(:,2),xbins);
colormap jet
grid on
xlabel('Energy');
title('DVertices');

%Dvertex energy vs Dodec energy and vs Correlation
figure
scatter(angles(:,4),doublevertices(:,1,2))
grid on
xlabel('Energy');
title('DVertices');

%% Youssef: Facetypes


load('data/evolve/faces'); %n*12*(central magnets, numberoftypes, face orientation, Energy)
%f= reshape(faces, [12*nsimul,10]);
f= zeros(12*nsimul,10);
for i= 1:length(faces)
    for j = (i*12 -11): i*12
        f(j,:) = faces(i,mod(j-1,12)+1,:);
    end
end


%face energy:

figure
C = unique(f(:,6));
A = zeros(length(f),length(C));
A = A*nan;
for i = 1:length(f)
    for j = 1:length(C)
        if f(i,6) == C(j)
            A(i,j) = f(i,10);
        end
    end
end
A(A==0) = nan;

xbins = -200:50;
xbins = xbins*10^-6;
hist(A(:,1:length(C)), xbins);
colormap jet
grid on
ylabel('Count');
xlabel('Energy')
title('Energy and number of WW on face');
legend(num2str(C(1:length(C))));

%Energy and Face type

C = unique(f(:,9));
A = zeros(length(f),length(C));
A = A*nan;
for i = 1:length(f)
    for j = 1:length(C)
        if f(i,9) == C(j)
            A(i,j) = f(i,10);
        end
    end
end
A(A==0) = nan;

figure
hist(A(:,1:length(C)),50);
colormap([jet(1); jet(2); jet(2); jet(12); jet(4); jet(14);jet(6); jet(16)]);
colormap(hsv(7))
grid on
xlabel('Energy');
ylabel('Count') 
title('Energy and Face type');


%FF and Face type
figure
C = unique(f(:,9));
A = zeros(length(f),length(C));
A = A*nan;
for i = 1:length(f)
    for j = 1:length(C)
        if f(i,9) == C(j)
            A(i,j) = f(i,8);
        end
    end
end
%A(A==0) = nan;

hist(A(:,1:length(C)));
colormap jet
grid on
xlabel('Faceorientation');
ylabel('Count')
title('FF-Facetype');
legend(num2str(C(1:length(C))));

%Detailed face types:
detailedfacetypes= f(:,1);

for i = 1:length(f)
    detailedfacetypes(i)=  f(i,6) + f(i,7)*10 +f(i,8)*100 + abs(f(i,9))*1000 ;
end

figure
xbin = 0:15000;
hist(detailedfacetypes,xbin);
colormap jet
grid on
xlabel('Face types (detailed)');
ylabel('Count')
title('Detailed Face types');

uniqueDetFacetypes = unique(detailedfacetypes);


% Important : Energy and (7) detailed Face type
figure
C = uniqueDetFacetypes;
A = zeros(length(f),length(C));
A = A*nan;
for i = 1:length(f)
    for j = 1:length(C)
        if detailedfacetypes(i) == C(j)
            A(i,j) = f(i,10);
        end
    end
end
%A(A==0) = nan;
A = A ;
xbins = xbins;
hist(A(:,1:length(C)),xbins);
colormap jet
grid on
xlabel('Energy [J]');
ylabel('Count')
title('Energy and Face type(detailed)');
%legend(num2str(C(1:length(C))));
legend('Frustrated face - 0FF 1FW 4WW', '(3-2) face - 0FF 3FW 2WW', '(3-2) face - 1FF 1WW 3WW', '(4-1) face - 0FF 3FW 2WW', '(4-1) face - 1FF 1FW 3WW', 'Perfect face -1FF 3FW 1WW', 'Perfect face - 2FF 1FW 2WW');

%% Youssef: Facetypes with detailed FF

%FF and Face type

C = uniqueDetFacetypes;
A = zeros(length(f)*5,length(C));
A = A*nan;
for i = 1:length(f)-1 
    for j = 1:length(C)
        for l = 1:5   
            if detailedfacetypes(i) == C(j)
            A(i+l,j) =m_ff_A(f(i,l)+1 +floor((i-1)/12)*30,2); 
            end
        end
    end
end
    

%A(A==0) = nan;
figure
hist(A,10000:25000);
colormap jet
grid on
xlabel('Facetype');
ylabel('Count')
title('FF-Facetype');
legend(num2str(C));

% Face type and FF
detailedfacetypes2 = detailedfacetypes;
C = (unique(m_ff_A(:,2)));
C = C(2:7);
A = zeros(length(f),length(C));
A = A*nan;
for i = 1:length(f) 
    for j = 1:length(C)
        for l = 1:5 
            ind =f(i,l)+1 +floor((i-1)/12)*30; %index on dode from face + index of dodec
            if  m_ff_A(ind,2) == C(j)
            A(i,j) =detailedfacetypes(i);
%             if abs(f(i,9)) == 10 %to discretize perfect face
                if 1==1
                    if floor(detailedfacetypes2(i)/10^5) >=j
                        detailedfacetypes2(i)= detailedfacetypes2(i)+j*10^6;
                    else
                        detailedfacetypes2(i)= detailedfacetypes2(i)+(j-floor(detailedfacetypes2(i)/10^5))*10^5   + floor(detailedfacetypes2(i)/10^5)*10^6; 
                    end
                end
            end
        end
    end
end
    

%A(A==0) = nan;
figure
hist(A,1000:15000);
colormap jet
grid on
xlabel('Facetype');
ylabel('Count')
title('FF-Facetype');
legend(num2str(C))


% Important? :Energy and detailed Face type
uniqueDetFacetypes2 = unique(detailedfacetypes2);

C = uniqueDetFacetypes2;
A = zeros(length(f),length(C));
A = A*nan;
for i = 1:length(f)
    for j = 1:length(C)
        if detailedfacetypes2(i) == C(j)
            A(i,j) = f(i,10);
        end
    end
end
%A(A==0) = nan;
figure
%range = [2,4,6]; %3-Face
range = [3,5,7,9]; %4-Face
%range = [17,18,21,22]; %perfect face
hist(A(:,range),30);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Energy and Perfect Face type(detailed)');
legend(num2str(C(range)));

figure
hist(detailedfacetypes2,1000);
colormap jet
grid on
xlabel('Face type');
ylabel('Count')
title('Histogram of Face type(detailed)');
%% Youssef: Facetypes with detailed FW

%FF and Face type

C = uniqueDetFacetypes;
A = zeros(nsimul*12*5,length(C));
A = A*nan;
for i = 1:nsimul*12-1 
    for j = 1:length(C)
        if C(j)== detailedfacetypes(i)
            for l = 1:5   
                A(i+l,j) =m_fw_A(f(i,l)+1 +floor((i-1)/12)*30,1); 
            end
        end
    end
end
    

A(A==0) = nan;
figure
hist(A);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('FW-Facetype');
legend(num2str(C));




%% Youssef: detailed FF and Energy of Dodec (not useful now)
DodecFFdetailed = zeros(length(dodecs(:,1)),1);
C = (unique(m_ff_A(:,2)));
C = C(2:7);
for i = 1: length( DodecFFdetailed)
    counter =0;
    for k =1:30
    for j = 1:length(C)
        if C(j) == m_ff_A((i-1)*30 + k,2)  %% only 11411
            DodecFFdetailed(i) = DodecFFdetailed(i) +10^(j-1);
            counter = counter +1;
        end
    end
    end
%         to remove FF<7:
    if(counter<6) 
        DodecFFdetailed(i) = 0;
    end
% DodecFFdetailed(i)= DodecFFdetailed(i)/6;
end

figure
hist(DodecFFdetailed,1000);
colormap jet
grid on
xlabel('Dodec type in terms of FF');
ylabel('Count')
title('Histogram of Dodec type in terms of FF');

uniqueDodecFF= unique(DodecFFdetailed);

%Dodec configuration and Energy
C = uniqueDodecFF;
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if DodecFFdetailed(i) == C(j)
            A(i,j) = angles(i,2);
        end
    end
end

figure
range = [2,5,6,8,9,12,13,21,29];%[2,6,9,13];%2:length(C); 
hist(A(:,range),50);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Energy - Dodec configuration');
legend(num2str(C(range)));


%% Youssef: Detailed FF+#FF distribution of energy and angle (Important)






%% Youssef: Dodec types (with detailed (7) face types)
dodecstg2 = zeros(length(faces),1);
for i =1:(length(faces))
    for j = (i*12 -11): (i*12)
        for l = 1:length(uniqueDetFacetypes)
            if uniqueDetFacetypes(l) == detailedfacetypes(j)
                dodecstg2(i)= dodecstg2(i) + 10^(l-1);
            end
        end
    end
end
uniquepossibleconfigurations2 = unique(dodecstg2);

%Dodec configuration and Energy
C = uniquepossibleconfigurations2;
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if dodecstg2(i) == C(j)
            A(i,j) = angles(i,2);
        end
    end
end

figure
hist(A(:,1:length(C)));
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Energy - Dodec configuration');
legend(num2str(C(1:length(C))));





%% Youssef: Dodec types (with basic face types)
%for every orientation equal and opposite orientation:
load('data/evolve/dodecs'); %n*dodectype
figure
xbins = 0:60;
hist(dodecs,xbins);
colormap jet
grid on
xlabel('Dodec type');
ylabel('Count')
title('Dodec types');
legend('10 -10','3 -3', '2 -2', '1 -1');

% grouping
dodecsg = dodecs;
for i = 1:length(dodecs)
   for j = 1:4
       d1 = floor(dodecs(i,j)/10);
       d2 = int16(mod(dodecs(i,j),10));
       dodecsg(i,j) = min(d2,d1)*10 + max(d1,d2);
   end
end
figure
xbins = 0:60;
hist(dodecsg,xbins);
colormap jet
grid on
xlabel('Dodec type (grouped)');
ylabel('Count')
title('Dodec types');
legend('10 -10','3 -3', '2 -2', '1 -1');

%total grouping:

dodecstg = dodecs(:,1);
for i = 1:length(dodecs)
       d1 = floor(dodecs(i,1)/10);
       d2 = int16(mod(dodecs(i,1),10));
       dodecstg(i) = (d1+d2)*1000;
       d1 = floor(dodecs(i,2)/10);
       d2 = int16(mod(dodecs(i,2),10));
       dodecstg(i) = dodecstg(i) +(d1+d2)*100;
       d1 = floor(dodecs(i,3)/10);
       d2 = int16(mod(dodecs(i,3),10));
       dodecstg(i) = dodecstg(i) +(d1+d2)*10;
       d1 = floor(dodecs(i,4)/10);
       d2 = int16(mod(dodecs(i,4),10));
       dodecstg(i) = dodecstg(i) +(d1+d2)*1;
end
figure
xbins = 0:7100;
xbins2 =[2000 3000 4000 5000 6000 7000] + 500;
% for i = 1:length(dodecstg)
% histogram(dodecstg(i),xbins);
% end
hist(dodecstg,xbins)
colormap jet
grid on
xlabel('Dodec type (totally grouped, digits 10-4-3-2)');
ylabel('Count')
title('Dodec types');
uniquepossibleconfigurations= unique(dodecstg);

%% Youssef: Basic face types Dodec configuration and Energy

C = uniquepossibleconfigurations;
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if dodecstg(i) == C(j)
            A(i,j) = angles(i,2);
        end
    end
end

figure
range = [5,10,13,14,15,16,19,21];
%  for i = 1:21
%  histogram(A(:,i),15);
%  hold on
%  end
%histogram(A(:,1:21));
hist(A(:, range),60)
colormap jet

grid on
xlabel('Energy');
ylabel('Count')
title('Energy - Dodec configuration');
legend([num2str(C(range))]);


%% Youssef: # Perfect faces - Energy (not useful now)

C = [2; 3; 4; 5; 6];
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
       d1 = floor(dodecs(i,1)/10);
       d2 = int16(mod(dodecs(i,1),10));
        if d1+ d2 == C(j)
            A(i,j) = angles(i,2);
        end
    end
end
meanE=C;
figure
hold on
%hist(A)
%subplot(2,3,1)
%title("#Perfect faces =2 ")
tmp = A(:,1);
tmp(tmp==0)= [];
meanE(1) = nanmean(tmp);
histogram(tmp,50,'FaceAlpha',0.5);
%subplot(2,3,2)
%title("#Perfect faces =3 ")
tmp = A(:,2);
tmp(tmp==0)= [];
meanE(2) = nanmean(tmp);
histogram(tmp,50,'FaceAlpha',0.5);
% subplot(2,3,3)
%title("#Perfect faces =4 ")
tmp = A(:,3);
tmp(tmp==0)= [];
meanE(3) = nanmean(tmp);
histogram(tmp,50,'FaceAlpha',0.5);
% subplot(2,3,4)
%title("#Perfect faces =5 ")
tmp = A(:,4);
tmp(tmp==0)= [];
meanE(4) = nanmean(tmp);
histogram(tmp,50,'FaceAlpha',0.5);
% subplot(2,3,5)
%title("#Perfect faces =6 ")
tmp = A(:,5);
tmp(tmp==0)= [];
meanE(5) = nanmean(tmp);
histogram(tmp,50,'FaceAlpha',0.5);
PerfectEnergy =A;
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Energy - Number of perfect faces');
legend(num2str(C(1:length(C))));


%% Youssef: # 4-faces - Energy (not useful now)

%C = [22,23,24,25,26,27,28,34,35,36,37,42,43,44,45,46,52,53,54,61,62]';
C= [2:8]';
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
       d1 = floor(mod(dodecstg(i,1),1000)/100);
       %d1 = floor(dodecstg(i,1)/100);
        if d1 == C(j)
            A(i,j) = angles(i,2);
        end
    end
end
meanE=C;
figure
%range = 12:16;
range= [1:length(C)];
hold on
hist(A(:,range))
% %subplot(2,3,1)
% %title("#Perfect faces =2 ")
% tmp = A(:,1);
% tmp(tmp==0)= [];
% meanE(1) = nanmean(tmp);
% histogram(tmp,50,'FaceAlpha',0.5);
% %subplot(2,3,2)
% %title("#Perfect faces =3 ")
% tmp = A(:,2);
% tmp(tmp==0)= [];
% meanE(2) = nanmean(tmp);
% histogram(tmp,50,'FaceAlpha',0.5);
% % subplot(2,3,3)
% %title("#Perfect faces =4 ")
% tmp = A(:,3);
% tmp(tmp==0)= [];
% meanE(3) = nanmean(tmp);
% histogram(tmp,50,'FaceAlpha',0.5);
% % subplot(2,3,4)
% %title("#Perfect faces =5 ")
% tmp = A(:,4);
% tmp(tmp==0)= [];
% meanE(4) = nanmean(tmp);
% histogram(tmp,50,'FaceAlpha',0.5);
% % subplot(2,3,5)
% %title("#Perfect faces =6 ")
% tmp = A(:,5);
% tmp(tmp==0)= [];
% meanE(5) = nanmean(tmp);
% histogram(tmp,50,'FaceAlpha',0.5);
ThreeFaceEnergy=A;
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Energy - Number of 3-faces');
legend(num2str(C(range)));




%% Youssef: Detailled Dodec:
%total groupingdetailed:

dodecstgd = dodecs(:,1);
for i = 1:length(dodecs)
    d1 = floor(dodecs(i,1)/10);
    d2 = int64(mod(dodecs(i,1),10));
    %        dodecstgd(i) = (d1+d2)*(d1+d2+1)/2 +d2;
    d3 = floor(dodecs(i,2)/10);
    d4 = int64(mod(dodecs(i,2),10));
    %        dodecstgd(i) = dodecstg(i) +((d3+d4)*(d3+d4+1)/2 +d4)*100;
    d5 = floor(dodecs(i,3)/10);
    d6 = int64(mod(dodecs(i,3),10));
    %        dodecstgd(i) = dodecstg(i) +((d5+d6)*(d5+d6+1)/2 +d6)*10000;
    d7 = floor(dodecs(i,4)/10);
    d8 = int64(mod(dodecs(i,4),10));
%        dodecstgd(i) = dodecstg(i) +((d7+d8)*(d7+d8+1)/2 +d8)*1000000;
%        a =0;
%        if(d1> d2)
%            a=1;
%        end
%        if(d2> d1)
%            a=2;
%        end
%        dodecstgd(i) = dodecstg(i)+a*1000000;


    dodecstgd(i) = dodecs(i,1)*1000000;
    dodecstgd(i) = dodecstgd(i) +dodecs(i,2)*10000;

    dodecstgd(i) = dodecstgd(i) +dodecs(i,3)*100;

    dodecstgd(i) = dodecstgd(i) +dodecs(i,4)*1;
end
uniquepossibleconfigurationsd= unique(dodecstgd);
indexeddodecs = dodecstgd;
for i = 1:length(dodecstgd)
    for j = 1:length(uniquepossibleconfigurationsd)
        if(uniquepossibleconfigurationsd(j) == dodecstgd(i) )
            indexeddodecs(i) =j;
        end
    end
end


% Now accounting for symmetry:
uniquepossibleconfigurationsdSym = uniquepossibleconfigurationsd ;
for i =1: length(uniquepossibleconfigurationsdSym)
    d1i = floor(mod(uniquepossibleconfigurationsdSym(i),10));
    d2i = floor(mod(uniquepossibleconfigurationsdSym(i),100)/10);
    d3i = floor(mod(uniquepossibleconfigurationsdSym(i),1000)/100);
    d4i = floor(mod(uniquepossibleconfigurationsdSym(i),10000)/1000);
    d5i = floor(mod(uniquepossibleconfigurationsdSym(i),10^5)/10^4);
    d6i = floor(mod(uniquepossibleconfigurationsdSym(i),10^6)/10^5);
    d7i = floor(mod(uniquepossibleconfigurationsdSym(i),10^7)/10^6);
    d8i = floor(mod(uniquepossibleconfigurationsdSym(i),10^8)/10^7);
    for j =1: length(uniquepossibleconfigurationsdSym)
        d1j= floor(mod(uniquepossibleconfigurationsdSym(j),10));
        d2j= floor(mod(uniquepossibleconfigurationsdSym(j),100)/10);
        d3j= floor(mod(uniquepossibleconfigurationsdSym(j),1000)/100);
        d4j= floor(mod(uniquepossibleconfigurationsdSym(j),10000)/1000);
        d5j = floor(mod(uniquepossibleconfigurationsdSym(j),10^5)/10^4);
        d6j= floor(mod(uniquepossibleconfigurationsdSym(j),10^6)/10^5);
        d7j= floor(mod(uniquepossibleconfigurationsdSym(j),10^7)/10^6);
        d8j= floor(mod(uniquepossibleconfigurationsdSym(j),10^8)/10^7);
        if i~= j && d1i == d2j && d1j == d2i && d3i == d4j && d3j == d4i && d5i == d6j && d5j == d6i && d7i == d8j && d7j == d8i
            uniquepossibleconfigurationsdSym(j) = uniquepossibleconfigurationsdSym(i);
        end

    end
end
uniquepossibleconfigurationsdSym = unique(uniquepossibleconfigurationsdSym);
uniquepossibleconfigurationsdSymWithsum= [uniquepossibleconfigurationsdSym zeros(length(uniquepossibleconfigurationsdSym ),1)];
for i =1: length(uniquepossibleconfigurationsdSym)
    d1i = floor(mod(uniquepossibleconfigurationsdSym(i),10));
    d2i = floor(mod(uniquepossibleconfigurationsdSym(i),100)/10);
    d3i = floor(mod(uniquepossibleconfigurationsdSym(i),1000)/100);
    d4i = floor(mod(uniquepossibleconfigurationsdSym(i),10000)/1000);
    d5i = floor(mod(uniquepossibleconfigurationsdSym(i),10^5)/10^4);
    d6i = floor(mod(uniquepossibleconfigurationsdSym(i),10^6)/10^5);
    d7i = floor(mod(uniquepossibleconfigurationsdSym(i),10^7)/10^6);
    d8i = floor(mod(uniquepossibleconfigurationsdSym(i),10^8)/10^7);
    uniquepossibleconfigurationsdSymWithsum(i,2)= d1i+d2i + 10*(d3i+d4i) + 100*(d5i+d6i) + 1000*(d7i+d8i);
end
uniquepossibleconfigurationsdSymWithsum = [uniquepossibleconfigurationsdSymWithsum ,zeros(length(uniquepossibleconfigurationsdSymWithsum),1)];
for i= 1:length(uniquepossibleconfigurationsdSymWithsum)
    for j= 1:length(uniquepossibleconfigurationsdSymWithsum)
       if uniquepossibleconfigurationsdSymWithsum(i,2) == uniquepossibleconfigurationsdSymWithsum(j,2) && i~=j
           uniquepossibleconfigurationsdSymWithsum(i,3) =uniquepossibleconfigurationsdSymWithsum(i,3)+1;
           %uniquepossibleconfigurationsdSymWithsum(j,3) =uniquepossibleconfigurationsdSymWithsum(j,3)+1;
       end
    end
end
figure
xbins = 0:55;
hist(indexeddodecs,xbins);
colormap jet
grid on
xlabel('Dodec type (indexed)');
ylabel('Count')
title('Dodec types');



%Dodec configuration and Energy

C = uniquepossibleconfigurationsdSym;
A = zeros(length(angles),length(C));
A = A*nan;
for i = 1:length(angles)
    d1 = floor(dodecs(i,1)/10);
    d2 = int64(mod(dodecs(i,1),10));
    d3 = floor(dodecs(i,2)/10);
    d4 = int64(mod(dodecs(i,2),10));
    d5 = floor(dodecs(i,3)/10);
    d6 = int64(mod(dodecs(i,3),10));
    d7 = floor(dodecs(i,4)/10);
    d8 = int64(mod(dodecs(i,4),10));
    for j = 1:length(C)
        d1j= floor(mod(uniquepossibleconfigurationsdSym(j),10));
        d2j= floor(mod(uniquepossibleconfigurationsdSym(j),100)/10);
        d3j= floor(mod(uniquepossibleconfigurationsdSym(j),1000)/100);
        d4j= floor(mod(uniquepossibleconfigurationsdSym(j),10000)/1000);
        d5j = floor(mod(uniquepossibleconfigurationsdSym(j),10^5)/10^4);
        d6j= floor(mod(uniquepossibleconfigurationsdSym(j),10^6)/10^5);
        d7j= floor(mod(uniquepossibleconfigurationsdSym(j),10^7)/10^6);
        d8j= floor(mod(uniquepossibleconfigurationsdSym(j),10^8)/10^7);
        if d1 == d2j && d1j == d2 && d3 == d4j && d3j == d4 && d5 == d6j && d5j == d6 && d7 == d8j && d7j == d8
            A(i,j) = angles(i,2);
        end
        if uniquepossibleconfigurationsdSym(j) == dodecstgd(i)
             A(i,j) = angles(i,2);
        end
    end
end

figure
hist(A);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Energy - Dodec configuration (with symmetry)');
legend(num2str(C(1:length(C))));

%Dodec configuration and FF (temp)

%faces120 =reshape(face, [nsimul, 12*10]);

C = uniquepossibleconfigurationsd;
A = zeros(length(faces),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if  C(j) == dodecstgd(i)
            A(i,j) =0;
            for l = 1:12
                A(i,j) = A(i,j)+ faces(i,l,8);
            end
            A(i,j) = A(i,j)/2;
        end
    end
end
FFdodecnonunique = A;
FFDodec = unique(A);
FFDodec(isnan(FFDodec)) = FFDodec(1);
FFDodec = unique(FFDodec);

figure
xbins = 0:25;
hist(A,xbins);
colormap jet
grid on
xlabel('FF');
ylabel('Count')
title('FF - Dodec configuration');
legend(num2str(C(1:length(C))));
%Dodec configuration and WW (temp)

%faces120 =reshape(face, [nsimul, 12*10]);

C = uniquepossibleconfigurationsd;
A = zeros(length(faces),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if  C(j) == dodecstgd(i)
            A(i,j) =0;
            for l = 1:12
                A(i,j) = A(i,j)+ faces(i,l,6);
            end
            A(i,j) = A(i,j)/2;
        end
    end
end

WWDodec = unique(A);
WWDodec(isnan(WWDodec)) = WWDodec(1);
WWDodec = unique(WWDodec);

figure
xbins = 0:25;
hist(A,xbins);
colormap jet
grid on
xlabel('WW');
ylabel('Count')
title('WW - Dodec configuration');
legend({num2str(C(1:length(C)))});

%% Youssef: FF and WW Energy (Important)

%FF energy
C = FFDodec;
C= flip(C);
FWtotalangle2 =zeros(nsimul,length(C));
WWtotalangle2 =zeros(nsimul,length(C));
FFAverageVertexEnergy=zeros(nsimul,length(C));
A = zeros(nsimul,length(C));
A = A*nan;
A2 = A;
for i = 1:length(angles)
    for j = 1:length(C)
        tmp =0;
            for l = 1:12
                tmp = tmp + faces(i,l,8);
            end
            tmp = tmp/2; %this is #FF
            if tmp == C(j)
                A(i,j) = angles(i,2);
                A2(i,j) = s_C(i);
                FWtotalangle2(i,j)= FWtotalangle(i);
                WWtotalangle2(i,j)= WWtotalangle(i);
                FFAverageVertexEnergy(i,j)= AverageVertexEnergy(i);
            end
    end
end

h= figure
hold on
for i = 1: length(C)
        tmp = A(:,i);
        %tmp() =[];
        histogram(tmp,60,'Normalization','pdf');
end
%hist(A,100);
colormap( h, flip(jet));
grid on
xlabel('Energy');
ylabel('Count')
lgd = legend(num2str(C(1:length(C))));
title(lgd,'\#FF')



FFEnergy =A;
FFCorrelation = A2;
WWtotalangle2(WWtotalangle2==0)= nan;
FWtotalangle2(FWtotalangle2==0)=nan;
FFAverageVertexEnergy(FFAverageVertexEnergy==0)=nan;

%WW energy

C = flip(WWDodec);
A = zeros(length(faces),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        tmp =0;
        for l = 1:12
            tmp = tmp + faces(i,l,6);
        end
        tmp = tmp/2;
        if tmp == C(j)
            A(i,j) = angles(i,2);
        end
    end
end

figure
hold on
for i = 1: length(C)
        tmp = A(:,i);
        %tmp() =[];
        histogram(tmp,60,'Normalization','pdf');
end
%hist(A);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('WW - Energy');
legend(num2str(C(1:length(C))));



%FF-WW

C = flip(FFDodec);
A = zeros(length(faces),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
                tmp =0;
            for l = 1:12
                tmp = tmp + faces(i,l,8);
            end
            tmp = tmp/2;

        if  C(j) == tmp
            A(i,j) =0;
            for l = 1:12
                A(i,j) = A(i,j)+ faces(i,l,6);
            end
            A(i,j) = A(i,j)/2;
        end
    end
end

figure
%hist(A);
hold on
for i = 1: length(C)
        tmp = A(:,i);
        %tmp() =[];
        histogram(tmp,10,'Normalization','pdf');
end
colormap jet
grid on
xlabel('WW');
ylabel('Count')
title('WW - FF');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'\#FF')

%% Youssef: #FF - total angle
C = FFDodec;
A = zeros(nsimul,length(C));
A = A*nan;
for i = 1:length(s_A)
    for j = 1:length(C)
        tmp =0;
            for l = 1:12
                tmp = tmp + faces(i,l,8);
            end
            tmp = tmp/2; %this is #FF
            if tmp == FFDodec(j)
                A(i,j) = s_A(i);
            end
    end
end


figure
range = 1:length(C); 
hist(A(:,range),50);
colormap jet
grid on
xlabel('total angle');
ylabel('Count')
title('\# FF - total angle');
legend(num2str(C(range)));


%% Youssef: #FF - total FF angle
C = FFDodec;
A = zeros(nsimul,length(C));
A = A*nan;
for i = 1:length(FFtotalangle)
    for j = 1:length(C)
        tmp =0;
            for l = 1:12
                tmp = tmp + faces(i,l,8);
            end
            tmp = tmp/2; %this is #FF
            if tmp == FFDodec(j)
                A(i,j) = FFtotalangle(i);
            end
    end
end


figure
range = 1:length(C); 
hist(A(:,range),50);
colormap jet
grid on
xlabel('total FF angle');
ylabel('Count')
title('\# FF - total FF angle');
legend(num2str(C(range)));
%% Youssef: #FF-(total angle-Energy)

%Dodectotalangle = zeros(4,nsimul,2); %4 #FFs, nsimul,2 parameters {energy, total angle}
C = FFDodec;
figure
hold on
for j =1: 4
scatter(s_A,FFEnergy(:,j));
end
colormap jet
grid on
ylabel('Energy [J] $10^{-4}$');
xlabel('Total angle')
title('\#FF-(total angle-Energy)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'FF')

%% Youssef: #FF-(total FF angle-Energy)

%Dodectotalangle = zeros(4,nsimul,2); %4 #FFs, nsimul,2 parameters {energy, total angle}
C = flip(FFDodec);
figure
hold on
for j =1: 4
scatter(FFtotalangle,FFEnergy(:,j),15);
end
colormap jet
grid on
ylabel('Energy [J] $10^{-4}$');
xlabel('Total FF angle [rad]')
title('\#FF-(total angle-Energy)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'\#FF')



%% Youssef: #FF-(total WW angle-Energy)

%Dodectotalangle = zeros(4,nsimul,2); %4 #FFs, nsimul,2 parameters {energy, total angle}
C = WWDodec;
figure
hold on
for j =1: 4
scatter(WWtotalangle,FFEnergy(:,j));
end
colormap jet
grid on
ylabel('Energy [J]');
xlabel('Total WW angle')
title('FF-(total WW angle-Energy)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'WW')

%% Youssef: #FF-(total WW angle-total FW angle) (important)

%Dodectotalangle = zeros(4,nsimul,2); %4 #FFs, nsimul,2 parameters {energy, total angle}
C = flip(FFDodec);
figure
hold on
for j =1: 4
scatter(WWtotalangle,FWtotalangle2(:,j));
end
colormap jet
grid on
ylabel('Total FW angle [rad]');
xlabel('Total WW angle [rad]')
title('\#FF-(Total WW angle-Total FF angle)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'FF')

%% Youssef: #FF-(total FW angle-Energy)

%Dodectotalangle = zeros(4,nsimul,2); %4 #FFs, nsimul,2 parameters {energy, total angle}
C = 20-2*FFDodec;
figure
hold on
for j =1: 4
scatter(FWtotalangle,FFEnergy(:,j));
end
colormap jet
grid on
ylabel('Energy');
xlabel('Total FW angle')
title('FF-(total FW angle-Energy)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'FF')

%% Youssef: #FF-(correlation-Energy)
C = FFDodec;

figure
hold on
for j =1: 4
scatter(s_C,FFEnergy(:,j));
end
colormap jet
grid on
xlabel('Energy');
ylabel('Correltaion')
title('FF-(correlation-Energy)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'FF')

%% Youssef: #FF-(Total FW angle + Total WW angle)v. Energy (Important) 

C = flip(FFDodec);

figure
hold on
for j =1: 4
scatter(WWtotalangle+FWtotalangle,FFEnergy(:,j),15);
end
colormap jet
grid on
xlabel('Total WW+FW angle [rad]');
ylabel('Energy [J]')
title('\#FF-(Total FW + WW angle)v. Energy');
 lgd = legend(num2str(C(1:length(C))));
 title(lgd,'\#FF')
 
 %% Youssef: #FF-(Total FW angle + Total WW angle)v. sum of Vertex Energy (Important) 

C = FFDodec;

figure
hold on
for j =1: 4
scatter(WWtotalangle+FWtotalangle,FFAverageVertexEnergy(:,j));
end
colormap jet
grid on
xlabel('Total angle WW+FW [rad]');
ylabel('Energy [J]')
title('\#FF-(Total FW angle + Total WW angle)v. VertexEnergy');
 lgd = legend(num2str(C(1:length(C))));
 title(lgd,'\#FF')
 
 %% Youssef: #FF-(Total FW angle + Total WW angle)v. Correlation (Important) 

C = FFDodec;

figure
hold on
for j =1: 4
scatter(FWtotalangle+ WWtotalangle,FFCorrelation(:,j));
end
colormap jet
grid on
xlabel('Total angle WW+FW [rad]');
ylabel('Correlation []')
title('\#FF-(Total FW angle + Total WW angle)v. Correlation');
 lgd = legend(num2str(C(1:length(C))));
 title(lgd,'\#FF')
 
 %% Youssef: #Perfect-face -(Total angle)
Dodecnumberofperfect = zeros(nsimul,1);
for i =1: nsimul
        d1 = int64(dodecs(i,1)/10);
        d2 = int64(mod(dodecs(i,1),10));
        numberofperfectfaces=d1+d2;
        Dodecnumberofperfect(i) = numberofperfectfaces;
end

C = unique(Dodecnumberofperfect);
A = zeros(nsimul,length(C));
A = A*nan;
for i = 1:length(s_A)
    for j = 1:length(C)
        d1 = int64(dodecs(i,1)/10);
        d2 = int64(mod(dodecs(i,1),10));
        numberofperfectfaces=d1+d2;
        if numberofperfectfaces == C(j)
            A(i,j) = s_A(i);
        end
    end
end

uniquenumberperfectface = C;

figure
range = 1:length(C); 
hist(A(:,range),50);
colormap jet
grid on
xlabel('total angle');
ylabel('Count')
title('\#Perfect - total angle');
legend(num2str(C(range)));

%% Youssef: #4-face -(Total angle)
Dodecnumberofthreeface = zeros(nsimul,1);
for i =1: nsimul
        d1 = floor(mod(dodecstg(i),1000)/100);
        
        numberofthreefaces=d1;
        Dodecnumberofthreeface(i) = numberofthreefaces;
end

C = unique(Dodecnumberofthreeface);
A = zeros(nsimul,length(C));
A = A*nan;
for i = 1:length(s_A)
    for j = 1:length(C)
        d1 = floor(mod(dodecstg(i),1000)/100);
        
        numberofthreefaces=d1;
        if numberofthreefaces == C(j)
            A(i,j) = s_A(i);
        end
    end
end

uniquenumberthreeface = C;

figure
range = 1:length(C); 
hist(A(:,range),50);
colormap jet
grid on
xlabel('total angle [rad]');
ylabel('Count')
title('\#4-Face - total angle');
legend(num2str(C(range)));

%% Youssef: #4-face -(Total FW angle + Total WW angle-Energy)

C = uniquenumberthreeface;
figure
hold on
for j =1: length(C)

    scatter( FWtotalangle + WWtotalangle,ThreeFaceEnergy(:,j));
end
colormap jet
grid on
ylabel('Energy [J] $10^{-4}$');
xlabel('Total angle')
title('\#4-face-(total FW +WW angle-Energy)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'\#4-face')

%% Youssef: #Perfect-face -(Total FW angle + Total WW angle-Energy)

C = uniquenumberperfectface;
figure
hold on
for j =1: length(C)
    scatter( s_C,PerfectEnergy(:,j));
    p= polyfit( s_C,PerfectEnergy(:,j));
    y= polyval(p,linspace(min(s_C),max(s_C),100));
    
end
colormap jet
grid on
ylabel('Energy [J] $10^{-4}$');
xlabel('Correlation')
title('\#Perfect-(Correlation-Energy)');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'\#Perfect')

%% Youssef: Orientation sum Dodec type
Orientationsum = zeros(length(dodecs),length(uniquepossibleconfigurationsd));
Orientationsum = Orientationsum*nan;
for i = 1:length(dodecs)
       d1 = floor(dodecs(i,1)/10);
        d2 = int64(mod(dodecs(i,1),10));
      d3 = floor(dodecs(i,2)/10);
       d4 = int64(mod(dodecs(i,2),10));
      d5 = floor(dodecs(i,3)/10);
      d6 = int64(mod(dodecs(i,3),10));
       d7 = floor(dodecs(i,4)/10);
       d8 = int64(mod(dodecs(i,4),10));
       Osum = (d1-d2)*5 + (d3-d4)*4 + (d5-d6)*3 + (d7-d8)*1;
    for j=1:length(uniquepossibleconfigurationsd)
        if(uniquepossibleconfigurationsd(j) == dodecstgd(i) )
            Orientationsum(i,j) =Osum;
        end
    end
end

figure
xbins = -11:11;
hist(Orientationsum(:,1:length(uniquepossibleconfigurationsd)),xbins);
colormap jet
grid on
xlabel('Orientation sum');
ylabel('Count')
title('Orientation sum - Dodec type');
lgd = legend(num2str(uniquepossibleconfigurationsd(1:length(uniquepossibleconfigurationsd))));
title(lgd,'Dodec type')


%% Youssef: Orientationsum Energy and FF

Orientationsum2 = zeros(nsimul,1);
for i = 1:length(dodecs)
       d1 = floor(dodecs(i,1)/10);
        d2 = int64(mod(dodecs(i,1),10));
      d3 = floor(dodecs(i,2)/10);
       d4 = int64(mod(dodecs(i,2),10));
      d5 = floor(dodecs(i,3)/10);
      d6 = int64(mod(dodecs(i,3),10));
       d7 = floor(dodecs(i,4)/10);
       d8 = int64(mod(dodecs(i,4),10));
       Osum = abs((d1-d2)*5 + (d3-d4)*4 + (d5-d6)*3 + (d7-d8)*1);
       Orientationsum2(i) = Osum;
end

C = unique(Orientationsum2);
A = zeros(length(faces),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if(Orientationsum2(i) == C(j))
            A(i,j) =angles(i,2);
        end
    end
end

figure
hist(A);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Osum - Energy');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'Osum')

%FF-Osum
C = unique(Orientationsum2);
A = zeros(length(faces),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if(Orientationsum2(i) == C(j))
            tmp =0;
            for l = 1:12
                tmp = tmp + faces(i,l,8);
            end
            A(i,j) =tmp/2;
        end
    end
end

figure
hist(A);
colormap jet
grid on
xlabel('FF');
ylabel('Count')
title('Osum - FF');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'Osum')

%% Youssef: moredetailed dodecconfig
load('data/evolve/dodecs');
dodecstgdmore = dodecs(:,1);
weird = [];
for i = 1:length(dodecs)
        dodecstgdmore(i) = dodecs(i,1);
        dodecstgdmore(i) = dodecstgdmore(i) +dodecs(i,2)*100;

        dodecstgdmore(i) = dodecstgdmore(i) +dodecs(i,3)*10000;

        dodecstgdmore(i) = dodecstgdmore(i) +dodecs(i,4)*1000000;
        numberofFF =0;
        for l = 1:12
                numberofFF = numberofFF + faces(i,l,6);
        end
        numberofFF = numberofFF/2;
        dodecstgdmore(i) = dodecstgdmore(i) + numberofFF*100000000;
end
uniquepossibleconfigurationsdmore= unique(dodecstgdmore);
indexeddodecsmore = dodecstgdmore;
for i = 1:length(dodecstgdmore)
    for j = 1:length(uniquepossibleconfigurationsdmore)
        if(uniquepossibleconfigurationsdmore(j) == dodecstgdmore(i) )
            indexeddodecsmore(i) =j;
        end
    end
end

figure
xbins = 0:550;
hist(indexeddodecsmore,xbins);
colormap jet
grid on
xlabel('Dodec type more detailed (indexed)');
ylabel('Count')
title('Dodec types (more detailed)');

%Energy
C = uniquepossibleconfigurationsdmore;
A = zeros(length(faces),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if dodecstgdmore(i) == uniquepossibleconfigurationsdmore(j)
            A(i,j) = angles(i,2);
        end

    end
end

figure
hist(A);
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Detailed dodec - Energy');
legend(num2str(C(1:length(C))));

%% FF and WW and FW and WF
%FF v. number of perfect faces:
C = FFDodec;
A = zeros(length(dodecs),length(C));
A = A*nan;
DodecPerfectFF = zeros(length(angles),1);
DodecThreeFF = zeros(length(angles),1);
DodecFF = zeros(length(angles),1);
for i = 1:length(angles)
    for j = 1:length(C)
        d1 = int64(dodecs(i,1)/10);
        d2 = int64(mod(dodecs(i,1),10));
        numberofperfectfaces=d1+d2;
        numberofThreefaces= floor(mod(dodecstg(i),1000)/100);
        numberofFF =0;
        for l = 1:12
            numberofFF = numberofFF + faces(i,l,8);
        end
        numberofFF = numberofFF/2;
        if C(j) == numberofFF
            A(i,j) = numberofperfectfaces;
            DodecPerfectFF(i) = numberofperfectfaces*10 + numberofFF;
            DodecThreeFF(i)= numberofThreefaces*10 + numberofFF;
            DodecFF(i) = numberofFF;
        end
    end
end

figure
xbin = 0:6;
hist(A,xbin);
colormap jet
grid on
xlabel('Numberofperfectfaces');
ylabel('Count')
title('FF - Numberofperfectfaces');
lgd = legend(num2str(C(1:length(C))));
title(lgd,'FF')




%% Youssef: Perfect faces + FF 
C= unique(DodecPerfectFF);
A = zeros(length(DodecPerfectFF),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if C(j) == DodecPerfectFF(i)
            A(i,j) = angles(i,2);
        end
    end
end

PerfectFFEnergy= A;
uniquePerfectFF = C;
figure
hold on
% ind =1;
% for i = 1: length(C)
%     if i~= 1 && i~= 4 && i~= 5 && i~= 8 && i~= 11 && i~= 14
%         subplot(3,3,ind)
%         title(num2str(C(i)))
%         ind = ind +1;
%         tmp = A(:,i);
%         %tmp() =[];
%         histogram(tmp,60,'Normalization','pdf');
%     end
% end
hist(A(:,[2,3,6,7,9,10,12,13,15]),30)

colormap jet
grid on
xlabel('Energy');
ylabel('Count')
%title('Detailed dodec () - Energy');
legend(num2str(C([2,3,6,7,9,10,12,13,15])));
%% Youssef: 4-faces + FF 
C= unique(DodecThreeFF);
A = zeros(length(DodecThreeFF),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if C(j) == DodecThreeFF(i)
            A(i,j) = angles(i,2);
        end
    end
end

ThreeFFEnergy= A;
uniqueThreeFF = C;
figure
range = [1:length(C)];
hold on
% ind =1;
% for i = 1: length(C)
%     if i~= 1 && i~= 4 && i~= 5 && i~= 8 && i~= 11 && i~= 14
%         subplot(3,3,ind)
%         title(num2str(C(i)))
%         ind = ind +1;
%         tmp = A(:,i);
%         %tmp() =[];
%         histogram(tmp,60,'Normalization','pdf');
%     end
% end
hist(A(:,range),30)

colormap jet
grid on
xlabel('Energy [J]');
ylabel('Count')
%title('Detailed dodec () - Energy');
legend(num2str(C(range)));

%% Youssef: Perfectface+ 3-Face

DodecP3 = zeros(length(angles),1);
for i = 1:length(angles)
    DodecP3(i) = 10*floor(dodecstg(i)/1000) +  floor(mod(dodecstg(i,1),1000)/100);
end

C= unique(DodecP3);
A = zeros(length(DodecP3),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if C(j) == DodecP3(i)
            A(i,j) = angles(i,2);
        end
    end
end
DodecP3Energy =A;
figure
hold on
% for i = 1: length(C)
%     if i~= 1 || i~= 4 || i~= 5 || i~= 8 || i~= 11 || i~= 14
%         tmp = A(:,i);
%         %histogram(tmp,30,'Normalization','pdf');
%     end
% end
 %hist(A(:,[2,3,6,7,9,10,12,13,15]),30)
hist(A)
colormap jet
grid on
xlabel('Energy');
ylabel('Count')
title('Detailed dodec (DodecPerfect-3Face) - Energy');
%legend(num2str(C([2,3,6,7,9,10,12,13,15])));
legend(num2str(C));

%% Youssef: #Perfect-faceFF -(Total FW angle + Total WW angle-Energy) (Important)

C = uniquePerfectFF;
figure
hold on
range = [3,7,10,13,15];
for j =range
    hold on
    scatter(FWtotalangle + WWtotalangle,PerfectFFEnergy(:,j));
    tmp= PerfectFFEnergy(:,j);
    tmp2=s_C;
    tmp2(isnan(tmp))=[];
    tmp(isnan(tmp))=[];
    p=polyfit( tmp2,tmp,1);
    x1 = 10^-1 *[00:10];
    y1=polyval(p,x1);
    %plot( x1,y1);
end
colormap jet
grid on
ylabel('Energy [J] $10^{-4}$');
xlabel('Total angle [rad]')
title('\#Perfect-(Total FW + WW angle-Energy)');
lgd = legend(num2str(floor(C(range)/10)));
%lgd=legend(num2str([2; 2; 3; 3; 4; 4; 5; 5; 6; 6]));
title(lgd,'\#Perfect-face')

%% Youssef: #4-faceFF -(Total FW angle + Total WW angle-Energy)

C = uniquePerfectFF;
figure
hold on
%range = [3,7,9,10,13];
range= 1:length(C);
for j =range
    scatter( FWtotalangle + WWtotalangle,ThreeFFEnergy(:,j));
end
colormap jet
grid on
ylabel('Energy [J] $10^{-4}$');
xlabel('Total angle')
title('PerfectFF-(total FW +WW angle-Energy)');
lgd = legend(num2str(C(range)));
title(lgd,'PerfectFF')


%% Youssef: DetailedFF+#FF - energy 
m_ffff_A= m_ff_A;

for i=1:nsimul
    for j=1:30
        m_ffff_A((i-1)*30 +j,2) = DodecFF(i)*10^5 + m_ffff_A((i-1)*30 +j,2);
    end
end
UniqueFFnumFF= unique(m_ffff_A(:,2));
x = m_ffff_A;

for i = 1:length(x)
    if x(i,1) == 0
        x(i,:) = 0;
    end
    x(i,1) = mod(x(i,1),pi);
    if x(i,1)>pi/2
        q = x(i,1);
        x(i,1) = pi - q;
        x(i,1) = mod(x(i,1),pi);
    end

end

C = unique(x(:,2));
A = zeros(length(x),length(C));
A = A*nan;

for i = 1:length(x)
    for j = 1:length(C)
        if x(i,2) == C(j)
            A(i,j) = x(i,1);
        end
    end
end



figure
%range = 2:length(C);
range= 10:length(C);
hist(A(:,range),50);
colormap jet
grid on
xlabel('angle [rad]');
ylabel('Count')
title('FF-\#FF angles');
legend(num2str(C(range-0)));



% Dvertex detailed - Energy:

C = unique(x(:,2));
A = zeros(length(x),length(C));
A = A*nan;
count = 0;
for i = 1:length(x)
    for j = 1:length(C)
        if x(i,2) == C(j)
            firstindex= (i -(mod(i-1,30)+1))/30 +1;
            for l = 1:30
                secondindex =mod(i-1,30);
                if secondindex == doublevertices(firstindex,l,1)
                A(i,j) = doublevertices(firstindex,l,2);
                end
            end
        end
    end
end

figure
xbins = [-270: -150]/9*10^-6;
%range = [2,12,7,15,14];
range = 10:length(C);
hist(A(:,range),xbins);
colormap jet
grid on
xlabel('Energy [J]');
ylabel('Count')
title('FF-\#FF Energy');
legend(num2str(C(range)));

%% Youssef: #strongFF
DodecstrongFF = zeros(length(angles),1);
for i = 1:length(angles)
    d1 = int64(dodecs(i,1)/10);
    d2 = int64(mod(dodecs(i,1),10));
    numberofperfectfaces=d1+d2;
    numberofstrongFF =0;
    for l = 1:30
        if(m_ff_A((i-1)*30 +l,2) == 11411 )
        numberofstrongFF = numberofstrongFF + 1;
        end
    end
    DodecstrongFF(i) = numberofstrongFF;
end

% Energy
C= unique(DodecstrongFF);
A = zeros(length(DodecstrongFF),length(C));
A = A*nan;
for i = 1:length(angles)
    for j = 1:length(C)
        if C(j) == DodecstrongFF(i)
            A(i,j) = angles(i,2);
        end
    end
end

StrongFFEnergy= A;
uniqueStrongFF = C;
figure
hold on
% ind =1;
% for i = 1: length(C)
%     if i~= 1 && i~= 4 && i~= 5 && i~= 8 && i~= 11 && i~= 14
%         subplot(3,3,ind)
%         title(num2str(C(i)))
%         ind = ind +1;
%         tmp = A(:,i);
%         %tmp() =[];
%         histogram(tmp,60,'Normalization','pdf');
%     end
% end
%hist(A(:,[2,3,6,7,9,10,12,13,15]),30)
range = [2: length(C)];
hist(A(:,range))
colormap jet
grid on
xlabel('Energy [J]');
ylabel('Count')
%title('Detailed dodec (DodecPerfectFF) - Energy');
legend(num2str(C(range)));

% Energy-total angle

%% Youssef: Vertex-#FF Energy
verticesDV1FF= verticesDV1;
for i =1: nsimul    
    for l= 1:20
        verticesDV1FF((i-1)*20 +l,2) = verticesDV1FF((i-1)*20 +l,2) +1000*DodecFF(i); 
    end    
end
x= verticesDV1FF;
C = unique(x(:,2));
A = zeros(length(x),length(C));
A = A*nan;
for i = 1:length(x)
    for j = 1:length(C)
        if x(i,2) == C(j)
            A(i,j) = x(i,1);
        end
    end
end
A(A==0) = nan;


figure
range= [length(C)-2, length(C)-7];
xbins = [-300: -160]/2 *10^-7;
hist(A(:,range),xbins);
colormap jet
grid on
xlabel('Energy [J]');
ylabel('Count')
title('Vertex Energy');
legend(num2str(C(range)));
 
%%

%vertices
load('data/evolve/vertices'); %nx30x(a,T)
v = reshape(vertices,[250,20,2]);%v_A = v(:,:,1:3);
%v_T = v(:,:,4:6);
v_E = v(:,:,1);

figure
histogram(v_E,20)

%%
%doublevertices
d = load('data/doublevertices'); %shape:(nsimul)x(30)x(anglex5,typex5,E)
d_A = d(:,:,1:5);
d_T = d(:,:,6:10);
d_E = d(:,:,11);

%faces
f = load('data/faces'); %shape:(nsimul)x(12)x(anglex5,Vtypex5,O,E)
f_A= f(:,:,1:5);
f_T = f(:,:,6:10);
f_E = f(:,:,11);
f_O = f(:,:,12);

%%%%%%% PLOTS %%%%%%%

%%scatter frequency plots%%
%s: all
%m: A, ff_A, fw_A, ww_A


%%histograms%%
%m_T
%v_T
%d_T
%f_T & f_T for each f_O

%%Gaussians (Box plots, show mean and std)%%
%T vs E for m,v,d,f

%%dependencies / correlations%%
%s: A, Aabs vs E
%s: A, Aabs vs C
%s: C vs E
%f_O vs f_E & f_O vs s_E

%think of good way to visualise frequency of face types, eg
%12 faces, each with one of four configs. Easiest is freq: number of perf faces


% function y = modpi(x)
%      x = x(x ~= 0);
%     x = mod(x,pi);
%     for i = 1:length(x)
%     if x(i)>pi/2
%         q = x(i);
%         x(i) = pi - q;
%     end
%     end
%     x = mod(x,pi);
%     y = x;
% end

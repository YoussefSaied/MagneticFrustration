%1. load files made by simulationt
%2. run analysis on a single line from data saved as finalanalysis.in
%3. save the files in the temp folder, load them and add to array of total
%data
%4. indices

%has to be named correctly.. include time
nsimul = 1000;

%% EVOLVE
executable = '/mnt/Shared/EPFL/BA5/project/progprojet/trial/TextandMatlab/Analysis';
evolve = load('data/evolve/angles'); %time, energy, KE, cor, anglesx30
magnets = zeros(nsimul,30,2); %angle, type
vertices = zeros(nsimul,20,4); %,energy, oddoneout
doublevertices = zeros(nsimul,30,3); %centralmag_index, energy, strengthdet
faces = zeros(nsimul,12,10); %index5,orientation,energy,
dodecs= zeros(nsimul,4);
for i = 1:nsimul
  tic
  angles = evolve(i,5:34);
  magnets(i,:,1) = angles;
  dlmwrite('finalangles',angles,'delimiter',' ');
  eval(sprintf('!%s',executable));

  %should create 5 temp files: Magnet, Vertex, DVertex, Faceparameters,
  %Dodectype
  tempvertex = load('temp/Vertex');
  tempdvertex = load('temp/DVertex');
  tempface = load('temp/Faceparameters');
  tempdodec = load('temp/Dodectype');
  for j = 1:30
      k = tempdvertex(j,1)+1; %index cm
      type = tempdvertex(j,3); %type
      magnets(i,k,2) = type;
  end
  vertices(i,:,:) = tempvertex;
  doublevertices(i,:,:) = tempdvertex(:,1:3);
  faces(i,:,:) = tempface;
  dodecs(i,:) = tempdodec; 
  toc
end

save('data/evolve/magnets','magnets');
save('data/evolve/vertices','vertices');
save('data/evolve/doublevertices','doublevertices');
save('data/evolve/faces','faces');
save('data/evolve/dodecs','dodecs');

%% EVOLVE1

evolve1 = load('data/evolve1/angles'); %time, energy, cor, anglesx30
magnets = zeros(nsimul,30,2); %angle, type
vertices = zeros(nsimul,20,2); %,energy, oddoneout
doublevertices = zeros(nsimul,30,3); %centralmag_index, energy, strengthdet
faces = zeros(nsimul,12,10); %index5,orientation,energy,
dodecs= zeros(nsimul,4);
for i = 1:nsimul  
  tic
  angles = evolve(i,5:34);
  magnets(i,:,1) = angles;
  dlmwrite('finalangles',angles,'delimiter',' ');
  eval(sprintf('!%s',executable));
  %should create 4 temp files: Magnet, Vertex, DVertex, Faceparameters
  %tempmagnet = load('Magnet');
  tempvertex = load('temp/Vertex');
  tempdvertex = load('temp/DVertex');
  tempface = load('temp/Faceparameters');
  tempdodec = load('temp/Dodectype');
  for j = 1:30
      k = tempdvertex(j,1); %index cm
      type = tempdvertex(j,3); %type
      magnets(i,k,2) = type;
  end
  vertices(i,:,:) = tempvertex;
  doublevertices(i,:,:) = tempdvertex;
  faces(i,:,:) = tempface;
  dodecs(i,:) = tempdodec; 
  toc
end
save('data/evolve1/magnets','magnets')
save('data/evolve1/vertices','vertices')
save('data/evolve1/doublevertices','doublevertices')
save('data/evolve1/faces','faces')
save('data/evolve1/dodecs','dodecs');


%% EVOLVE2

evolve = load('data/evolve2/angles'); %time, energy, cor, anglesx30
magnets = zeros(nsimul,30,2); %angle, type
vertices = zeros(nsimul,20,2); %,energy, oddoneout
doublevertices = zeros(nsimul,30,3); %centralmag_index, energy, strengthdet
faces = zeros(nsimul,12,7); %index5,orientation,energy,
for i = 1:nsimul
  angles = evolve2(i,4:33);
  magnets(i,:,1) = angles;
  dlmwrite('finalangles.in',angles,'delimiter',' ');
  system('debug/Analysis');
  %should create 4 temp files: Magnet, Vertex, DVertex, Faceparameters
  %tempmagnet = load('Magnet');
  tempvertex = load('Vertex');
  tempdvertex = load('DVertex');
  tempface = load('Faceparameters');
  for j = 1:30
      k = tempdvertex(j,1); %index cm
      type = tempdvertex(j,3); %type
      magnets(i,k,2) = type;
  end
  vertices(i,:,:) = tempvertex;
  doublevertices(i,:,:) = tempdvertex;
  faces(i,:,:) = tempface;
end
save('data/evolve2/magnets','magnets')
save('data/evolve2/vertices','vertices')
save('data/evolve2/doublevertices','doublevertices')
save('data/evolve2/faces','faces')

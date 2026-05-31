%disp('************** load **********************')
%load timestamp
%EtatPiste;
%Id;
%disp('************** Fin load **********************')

%t2BaseType_4_oIntegerEtatPiste(:,1:2) = [];
%alObject2BaseType_4_oIntegerId(:,1:2) = [];

format long g
figure, hold on, grid on, 

for i = 1: size(etatPiste, 2)
    
    t1 = find(etatPiste(:,i) == 0);
    %timestamp(t1)
    %id(t1,i)
    %plot3(timestamp(t1), ones(1,length(t1))*(i-1) , alObject2BaseType_4_oIntegerId(t1), '.k')
    plot(timestamp(t1), id(t1,i), '.c')
    
    t1 = find(etatPiste(:,i) == 1);
    %plot3(timestamp(t1), ones(1,length(t1))*(i-1), alObject2BaseType_4_oIntegerId(t1), '.g')
    plot(timestamp(t1), id(t1,i), '.g')
    
    t1 = find(etatPiste(:,i) == 2);
    %plot3(timestamp(t1), ones(1,length(t1))*(i-1), alObject2BaseType_4_oIntegerId(t1), '.r')
    plot(timestamp(t1), id(t1,i), '.y')
    
    t1 = find(etatPiste(:,i) == 3);
    %plot3(timestamp(t1), ones(1,length(t1))*(i-1), alObject2BaseType_4_oIntegerId(t1), '.m')
    plot(timestamp(t1), id(t1,i), '.m')
    
    t1 = find(etatPiste(:,i) == 4);
    %plot3(timestamp(t1), ones(1,length(t1))*(i-1), alObject2BaseType_4_oIntegerId(t1), '.y')
    plot(timestamp(t1), id(t1,i), '.','Color',[1 0.5 0])
    
    t1 = find(etatPiste(:,i) == 5);
    %plot3(timestamp(t1), ones(1,length(t1))*(i-1), alObject2BaseType_4_oIntegerId(t1), '.c')
    plot(timestamp(t1), id(t1,i), '.r')
    
    t1 = find(etatPiste(:,i) == 6);
    %plot3(timestamp(t1), ones(1,length(t1))*(i-1), alObject2BaseType_4_oIntegerId(t1), '.c')
    plot(timestamp(t1), id(t1,i), '.b')
    
    %pause
end
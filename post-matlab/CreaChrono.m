EtatPiste;
Id;
%X;
%Y;
VectorSize;
%NumeroPiste;

% on récupère le nombre de pistes

vectorSize = lObject2BaseType_4_oVectorSize(:,3);
%N = size(vectorSize,1);
M = size(t2BaseType_4_oIntegerEtatPiste,2)
timestamp = lObject2BaseType_4_oVectorSize(:,1)/1000;
%for I = 1:N
%    nbVector = vectorSize(I);

    % creation des vecteurs contenant les variables
    for J = 1:M-2,
        
        etatPiste(:,J) = t2BaseType_4_oIntegerEtatPiste(:,2+J);
        %if J <= vectorSize(J)
         %   id(:,J) = alObject2BaseType_4_oIntegerId(:,2+J)+0.5;
        %else
        id(:,J) = alObject2BaseType_4_oIntegerId(:,2+J);
        t1 = find(id(:,J) == 0);
        id(t1,J)= 0.1*J;
            
        %end
        %y(:,J) = cRealObject2BaseType_4_oFloatY(:,2+J);
        %x(:,J) = cRealObject2BaseType_4_oFloatX(:,2+J);
        %numeroPiste(:,J) = BaseType_4_oIntegerNumeroPiste(:,2+J);
    end
%end

%for i = 1 : length(timestamp), plot(timestamp(i),id(i),'Color',[id(i)/1000 0 0]), end

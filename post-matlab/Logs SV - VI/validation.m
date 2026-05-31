final(:,1)= ID_SV_transpose(:,1);

for idx = 1:length(valeurs_id)
%Cible confirmée
        range_final(idx,1) = rangeSV(idx,1);
        range_final(idx,2) = rangeVI(idx,2);
        range_final(idx,3) = rangeSV(idx,3);
        
        
        
        %Regénère vecteur propre
        for i = 1:length(ID_SV_transpose)
            if (ID_SV_transpose(i,1) >= range_final(idx,1) & (ID_SV_transpose(i,1) <= range_final(idx,2)))
                final(i,2) = range_final(idx,3);
            end
        end
end


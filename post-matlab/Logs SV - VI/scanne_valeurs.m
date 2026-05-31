for idx = 1:length(valeurs_id)

    %VI
        %Début
        for i = 1:length(ID_VI)
            if (ID_VI(i,2) == valeurs_id(idx))
                rangeVI(idx,3) = valeurs_id(idx);
                rangeVI(idx,1)=ID_VI(i,1);
                j=i;
                break
            end
        end
        
        %Fin
        for i = j+1:length(ID_VI)
            if (ID_VI(i,2) == valeurs_id(idx))
                rangeVI(idx,2)=ID_VI(i,1);
            end
        end


%SV

        %Début
        for i = 1:length(ID_SV)
            if (ID_SV(i,2) == valeurs_id(idx))
                rangeSV(idx,3) = valeurs_id(idx);
                rangeSV(idx,1)=ID_SV(i,1);
                j=i;
                break
            end
        end
        
        %Fin
        for i = j+1:length(ID_SV)
            if (ID_SV(i,2) == valeurs_id(idx))
                rangeSV(idx,2)=ID_SV(i,1);
            end
        end
end
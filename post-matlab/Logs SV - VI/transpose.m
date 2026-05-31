ID_SV_transpose = zeros(length(ID_VI),2);

ID_SV_transpose(:,1) = ID_VI(:,1);

for idx = 1:length(valeurs_id)
    for i=2:(length(ID_SV_transpose)-1)
        if ((ID_SV_transpose(i-1,1) > (rangeSV(idx,1)+1)) & (ID_SV_transpose(i+1,1) < (rangeSV(idx,2)-1)))
            ID_SV_transpose(i,2) = rangeSV(idx,3);
        end
    end
end



ID_VI_transpose = zeros(length(ID_VI),2);
ID_VI_transpose(:,1) = ID_VI(:,1);

for idx = 1:length(valeurs_id)
    for i=2:(length(ID_VI_transpose)-1)
        if ((ID_VI_transpose(i-1,1) > (rangeVI(idx,1)+1)) & (ID_VI_transpose(i+1,1) < (rangeVI(idx,2)-1)))
            ID_VI_transpose(i,2) = rangeVI(idx,3);
        end
    end
end
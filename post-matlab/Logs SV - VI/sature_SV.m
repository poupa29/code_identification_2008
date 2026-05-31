for i=1:length(ID_SV)
    if ((ID_SV(i,2)>10)|(ID_SV(i,2)==1))
        ID_SV(i,2)=20;
    end
end;
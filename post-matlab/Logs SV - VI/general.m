ID_SV = LivicFiltreSV_VI_8_oSV;
ID_VI = VI_dec_oIntegerId;
valeurs_id = [7 9 20];

ID_SV(:,2) = [];
ID_VI(:,2) = [];

for i=length(ID_SV):3
    ID_SV(:,i) = [];
end

for i=length(ID_VI):3
    ID_VI(:,i) = [];
end

sature_SV;
scanne_valeurs;
transpose;
validation;


plot(ID_VI_transpose(:,1),ID_VI_transpose(:,2),'r')
hold on
plot(ID_SV_transpose(:,1),ID_SV_transpose(:,2)+1,'b')
plot(final(:,1),final(:,2)+2,'g')
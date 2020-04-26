#include <pbc.h>
#include <pbc_test.h>
#include <time.h>
#define N 129
int main(int argc, char **argv) {
	pairing_t pairing;
	element_t s,r_ID,h_ID,s_ID,x_ID,u,t,r,h2,u_s,z;
	element_t P,Ppub,R_ID,P_ID,Q_ID,T;
	double Time[60]={0},sum=0;
	int i;

	pbc_demo_pairing_init(pairing, argc, argv);
	element_init_Zr(s,pairing);//��������ʼ��ΪZr�ϵ�Ԫ��
	element_init_Zr(r_ID,pairing);
	element_init_Zr(h_ID, pairing);
	element_init_Zr(s_ID, pairing);
	element_init_Zr(x_ID, pairing);
	element_init_Zr(u, pairing);
	element_init_Zr(t, pairing);
	element_init_Zr(r, pairing);
	element_init_Zr(h2, pairing);
	element_init_Zr(u_s, pairing);
	element_init_Zr(z, pairing);
	element_init_G1(P,pairing);//��������ʼ��ΪG1�ϵ�Ԫ��
	element_init_G1(Ppub,pairing);
	element_init_G1(R_ID,pairing);
	element_init_G1(P_ID,pairing);
	element_init_G1(Q_ID,pairing);
	element_init_G1(T,pairing);
	
	element_t z_v,r_v,h_ID_v,h2_v;
	element_t T_v,P_left,P_rignt,Temp;
	element_init_Zr(z_v,pairing);
	element_init_Zr(r_v,pairing);
	element_init_Zr(h_ID_v,pairing);
	element_init_Zr(h2_v,pairing);
	element_init_G1(T_v,pairing);
	element_init_G1(P_left,pairing);
	element_init_G1(P_rignt,pairing);
	element_init_G1(Temp,pairing);
	
	clock_t begintime,endtime;
	for(i=0;i<60;i++){
	begintime=clock();	
	//��ʱ��ʼ
	//����˽Կ��ȡ
	element_random(s);
	element_random(P);
	element_mul_zn(Ppub,P,s);//����Ppub=sP
	
	element_random(r_ID);
	element_mul_zn(R_ID,P,r_ID);//����R_ID=(r_ID)P
	
	char ID_R[N+10]="0x0000006";
	unsigned char R_bit1[N]="0";
	element_to_bytes(R_bit1, R_ID);
	strcat(ID_R,(const char *)R_bit1);
	element_from_hash(h_ID,ID_R,strlen(ID_R));//����h_ID=H1(ID,R_ID)
	
	element_mul(s_ID,s,h_ID);
	element_add(s_ID,r_ID,s_ID);
	
	element_random(x_ID);
	element_mul_zn(P_ID,P,x_ID);//����P_ID=(x_ID)P
	unsigned char P_ID_bit1[N]="0";
	element_to_bytes(P_ID_bit1, P_ID);
	char ID_P[N+10]="0x0000006";
	strcat(ID_P,(const char *)P_ID_bit1);
	element_from_hash(u,ID_P,strlen(ID_P));//����u=H2(ID,P_ID)
	element_mul_zn(Q_ID,P_ID,u);
	element_add(Q_ID,Q_ID,R_ID);
	
	//ǩ��
	element_random(t);
	element_mul_zn(T,P,t);//����T=tP
	
	int n=element_length_in_bytes_x_only(T);
	unsigned char *data=(unsigned char *)malloc(n);
	element_to_bytes_x_only(data,T);
	element_from_bytes(r,data);
	free(data);//����r=Tx
	
	char ID_P_s[N+10]="0x0000006";
	unsigned char P_ID_bit2[N]="0";
	element_to_bytes(P_ID_bit2, P_ID);
	strcat(ID_P_s,(const char *)P_ID_bit2);
	element_from_hash(u_s,ID_P_s,strlen(ID_P_s));//����u=H2(ID,P_ID)

	char ID_h2[5*N]="0x0000006";
	char M[N]="abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	strcat(ID_h2,M);
	int m=element_length_in_bytes(h_ID);
	unsigned char *hID1=(unsigned char *)malloc(m);
	element_to_bytes(hID1,h_ID);
	const char *hID2=(const char *)hID1;
	strcat(ID_h2,hID2);
	free(hID1);
	unsigned char R_ID_bit2[N]="0";
	element_to_bytes(R_ID_bit2, R_ID);
	strcat(ID_h2,(const char *)R_ID_bit2);
	unsigned char Q_ID_bit[N]="0";
	element_to_bytes(Q_ID_bit, Q_ID);
	strcat(ID_h2,(const char *)Q_ID_bit);
	unsigned char T_bit[N]="0";
	element_to_bytes(T_bit, T);
	strcat(ID_h2,(const char *)T_bit);
	element_from_hash(h2,ID_h2,strlen(ID_h2));//����v=H3(ID,m,h_ID,R_ID,Q_ID,T)
	
	element_mul(z,u_s,x_ID);
	element_add(z,z,s_ID);
	element_mul(z,z,r);
	element_add(z,z,h2);
	element_invert(t,t);
	element_mul(z,z,t);
	
	//��֤ǩ��
	element_set(T_v,T);
	element_set(z_v,z);
	
	int n_v=element_length_in_bytes_x_only(T_v);
	unsigned char *data_v=(unsigned char *)malloc(n_v);
	element_to_bytes_x_only(data_v,T_v);
	element_from_bytes(r_v,data_v);
	free(data_v);//����r=Tx
	
	unsigned char R_bit_v[N]="0";
	element_to_bytes(R_bit_v, R_ID);
	char ID_R_v[N+10]="0x0000006";
	strcat(ID_R_v,(const char *)R_bit_v);
	element_from_hash(h_ID_v,ID_R_v,strlen(ID_R_v));//����h_ID_v=H1(ID,R_ID)
	
	char ID_h2_v[5*N]="0x0000006";
	char M_v[N]="abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	strcat(ID_h2_v,M_v);
	int m_v=element_length_in_bytes(h_ID_v);
	unsigned char *hID1_v=(unsigned char *)malloc(m_v);
	element_to_bytes(hID1_v,h_ID_v);
	const char *hID2_v=(const char *)hID1_v;
	strcat(ID_h2_v,hID2_v);
	free(hID1_v);
	strcat(ID_h2_v,(const char *)R_bit_v);
	unsigned char Q_ID_bit_v[N]="0";
	element_to_bytes(Q_ID_bit_v, Q_ID);
	strcat(ID_h2_v,(const char *)Q_ID_bit_v);
	unsigned char T_bit_v[N]="0";
	element_to_bytes(T_bit_v, T_v);
	strcat(ID_h2_v,(const char *)T_bit_v);
	element_from_hash(h2_v,ID_h2_v,strlen(ID_h2_v));//����v=H3(ID,m,h_ID,R_ID,Q_ID,T)
	
	element_mul_zn(P_rignt,Ppub,h_ID_v);
	element_add(P_rignt,P_rignt,Q_ID);
	element_mul_zn(P_rignt,P_rignt,r_v);
	element_mul_zn(Temp,P,h2_v);
	element_add(P_rignt,P_rignt,Temp);
	
	element_mul_zn(P_left,T_v,z_v);

	bool isVerify=!element_cmp(P_left,P_rignt); 
	//��ʱ����
	endtime = clock();		
	if(isVerify)
		Time[i]=(double)(endtime-begintime)/CLOCKS_PER_SEC;
	}

	printf("\n");
	for(i=0;i<60;i++)
		printf("%lf, ",Time[i]);//�ɸ���t1[i]�Ƿ�Ϊ0�ж���֤�Ƿ�ɹ�
	printf("\n");
	for(i=0;i<60;i++)
		sum+=Time[i];	
	printf("averagetime1=%lf\n",sum/60);

	element_clear(s);
	element_clear(r_ID);
	element_clear(h_ID);
	element_clear(s_ID);
	element_clear(x_ID);
	element_clear(u);
	element_clear(t);
	element_clear(r);
	element_clear(h2);
	element_clear(u_s);
	element_clear(z);
	element_clear(P);
	element_clear(Ppub);
	element_clear(R_ID);
	element_clear(P_ID);
	element_clear(Q_ID);
	element_clear(T);
	element_clear(T_v);
	element_clear(z_v);
	element_clear(r_v);
	element_clear(h_ID_v);
	element_clear(h2_v);
	element_clear(P_left);
	element_clear(P_rignt);
	element_clear(Temp);
	return 0;
}

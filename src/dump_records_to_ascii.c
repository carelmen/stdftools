/**
 * @file dump_records_to_ascii.c
 */
/*
 * Copyright (C) 2004-2006 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header: /cvsroot/freestdf/libstdf/examples/dump_records_to_ascii.c,v 1.23 2005/12/08 04:07:43 vapier Exp $
 */

#include <libstdf.h>

//#define print_fmt(n,f,v) printf("\t" n ": " f, v)
#define print_fmt(n,f,v) printf(f, v)
#define print_int(n,i) print_fmt(n, "%i", i)
#define print_str(n,s) print_fmt(n, "%s", (*(s) ? (s)+1 : "(null)"))
#define print_chr(n,c) print_fmt(n, "%c", c)
#define print_hex(n,h) print_fmt(n, "%X", h)
#define print_rel(n,r) print_fmt(n, "%f", r)
#define print_tim(n,d) \
	do { time_t t = d; print_fmt(n, "%s", ctime(&t)); } while(0)

#define MAKE_PRINT_X(DTC, OUTPUT_FUNC, FORMAT) \
void print_x ## DTC(char *n, dtc_x ## DTC u, dtc_U2 c) \
{ \
	dtc_U2 i; \
	printf("\t%s: ", n); \
	for (i=0; i<c; ++i) { \
		OUTPUT_FUNC(FORMAT, u[i]); \
		if (i+1 < c) printf(", "); \
	} \
	printf("\n"); \
}
MAKE_PRINT_X(U1, printf, "%u")
MAKE_PRINT_X(U2, printf, "%u")
MAKE_PRINT_X(R4, printf, "%f")

#define _printf_xCn(fmt,Cn) printf(fmt, (*Cn ? Cn+1 : "(null)"))
MAKE_PRINT_X(Cn, _printf_xCn, "%s")

void print_xN1(char *member, dtc_xN1 xN1, dtc_U2 c)
{
	dtc_N1 *n = xN1;
	printf("\t%s: ", member);
	while (c > 0) {
		if (c > 1) {
			printf("%X %X ", ((*n) & 0xF0) >> 4, (*n) & 0x0F);
			c -= 2;
		} else {
			printf("%X", ((*n) & 0xF0) >> 4);
			break;
		}
		++n;
	}
	printf("\n");
}

void print_Vn(char *n, dtc_Vn v, int c)
{
	int i;
	--c;
	printf("\t%s:\n", n);
	for (i=0; i<=c; ++i) {
		printf("\t\t%s: ", stdf_get_Vn_name(v[i].type));
		switch (v[i].type) {
			case GDR_B0: printf("(pad)"); break;
			case GDR_U1: printf("%i", *((dtc_U1*)v[i].data)); break;
			case GDR_U2: printf("%i", *((dtc_U2*)v[i].data)); break;
			case GDR_U4: printf("%i", *((dtc_U4*)v[i].data)); break;
			case GDR_I1: printf("%i", *((dtc_I1*)v[i].data)); break;
			case GDR_I2: printf("%i", *((dtc_I2*)v[i].data)); break;
			case GDR_I4: printf("%i", *((dtc_I4*)v[i].data)); break;
			case GDR_R4: printf("%f", *((dtc_R4*)v[i].data)); break;
			case GDR_R8: printf("%f", *((dtc_R8*)v[i].data)); break;
			case GDR_Cn: {
				dtc_Cn Cn = *((dtc_Cn*)v[i].data);
				printf("%s", (*Cn ? Cn+1 : "(null"));
				break;
			}
			case GDR_Bn: printf("[??]"); break;
			case GDR_Dn: printf("[??]"); break;
			case GDR_N1: printf("%X", *((dtc_N1*)v[i].data)); break;
		}
		printf("\n");
	}
	if (c == -1)
		printf("\n");
}
void print_Bn(dtc_C1 *n, dtc_Bn b)
{
	int i;
	printf("\t%s:", n);
	for (i=1; i<=*b; ++i)
		printf(" %X", *(b+i));
	if (*b == 0)
		printf(" (null)");
	printf("\n");
}
void print_Dn(dtc_C1 *n, dtc_Dn d)
{
	int i;
	dtc_U2 *num_bits = (dtc_U2*)d, len;
	len = *num_bits / 8;
	if (*num_bits % 8) ++len;
	printf("\t%s:", n);
	for (i=2; i<len; ++i)
		printf(" %X", *(d+i));
	if (len == 0)
		printf(" (null)");
	printf("\n");
}

#define print_UNK(n) \
	do { \
		fprintf(stderr, "******************************************\n"); \
		fprintf(stderr, "This field (" n ") has not been tested!\n"); \
		fprintf(stderr, "Please consider sending this file to\n"); \
		fprintf(stderr, "vapier@gmail.com to help out the\n"); \
		fprintf(stderr, "FreeSTDF project and make sure this code\n"); \
		fprintf(stderr, "works exactly the way it should!\n"); \
		fprintf(stderr, "******************************************\n"); \
	} while (0)

int main(int argc, char *argv[])
{
	stdf_file *f;
	char *recname;
	rec_unknown *rec;
	int i;
	dtc_U4 stdf_ver;


	char lotinfo[128];
	char tester[64];
	char timestamp[64];
    char *s;

	if (argc <= 1) {
		printf("Need some files to open!\n");
		return EXIT_FAILURE;
	}

for (i=1; i<argc; ++i) {
	printf("Dumping %s\n", argv[i]);
	f = stdf_open(argv[i]);
	if (!f) {
		perror("Could not open file");
		continue;
	}
	stdf_get_setting(f, STDF_SETTING_VERSION, &stdf_ver);

	while ((rec=stdf_read_record(f)) != NULL) {
		recname = stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB);
		// printf("Record %s (%3i,%3i) %i bytes:\n", recname, rec->header.REC_TYP,
		// rec->header.REC_SUB, rec->header.REC_LEN);
		switch (HEAD_TO_REC(rec->header)) {

			case REC_MIR: {
				rec_mir *mir = (rec_mir*)rec;

                int8_t *field_len = (int8_t *)mir->LOT_ID;
				//printf("field sblot %d\n", *field_len);

     		//	 snprintf(lotinfo, *field_len, "%s\n", (mir->LOT_ID)+1);
                 s = mir->SBLOT_ID;
     			 sprintf(lotinfo, "%s ", (*(s) ? (s)+1 : "(null)"));
                s = mir->LOT_ID;
                sprintf(lotinfo, "%s %s ", lotinfo, (*(s) ? (s)+1 : "(null)"));
                s = mir->NODE_NAM;
                sprintf(lotinfo, "%s %s ", lotinfo, (*(s) ? (s)+1 : "(null)"));

               // print_str(",", mir->LOT_ID);
//
//				print_tim("SETUP_T", mir->SETUP_T);
//				print_tim("START_T", mir->START_T);
//				print_int("STAT_NUM", mir->STAT_NUM);
//				print_chr("MODE_COD", mir->MODE_COD);
//				print_chr("RTST_COD", mir->RTST_COD);
//				print_chr("PROT_COD", mir->PROT_COD);
//				print_int("BURN_TIM", mir->BURN_TIM);
//				print_chr("CMOD_COD", mir->CMOD_COD);
//				print_str("LOT_ID", mir->LOT_ID);
//				print_str("PART_TYP", mir->PART_TYP);
//				print_str("NODE_NAM", mir->NODE_NAM);
//				print_str("TSTR_TYP", mir->TSTR_TYP);
//				print_str("JOB_NAM", mir->JOB_NAM);
//				print_str("JOB_REV", mir->JOB_REV);
//				print_str("SBLOT_ID", mir->SBLOT_ID);
//				print_str("OPER_NAM", mir->OPER_NAM);
//				print_str("EXEC_TYP", mir->EXEC_TYP);
//				print_str("EXEC_VER", mir->EXEC_VER);
//				print_str("TEST_COD", mir->TEST_COD);
//				print_str("TST_TEMP", mir->TST_TEMP);
//				print_str("USER_TXT", mir->USER_TXT);
//				print_str("AUX_FILE", mir->AUX_FILE);
//				print_str("PKG_TYP", mir->PKG_TYP);
//				print_str("FAMILY_ID", mir->FAMILY_ID);
//				print_str("DATE_COD", mir->DATE_COD);
//				print_str("FACIL_ID", mir->FACIL_ID);
//				print_str("FLOOR_ID", mir->FLOOR_ID);
//				print_str("PROC_ID", mir->PROC_ID);
//				print_str("OPER_FRQ", mir->OPER_FRQ);
//				print_str("SPEC_NAM", mir->SPEC_NAM);
//				print_str("SPEC_VER", mir->SPEC_VER);
//				print_str("FLOW_ID", mir->FLOW_ID);
//				print_str("SETUP_ID", mir->SETUP_ID);
//				print_str("DSGN_REV", mir->DSGN_REV);
//				print_str("ENG_ID", mir->ENG_ID);
//				print_str("ROM_COD", mir->ROM_COD);
//				print_str("SERL_NUM", mir->SERL_NUM);
//				print_str("SUPR_NAM", mir->SUPR_NAM);

				break;
			}

			case REC_PRR: {
				rec_prr *prr = (rec_prr*)rec;
                // Get the bins and part id information
                 printf("%s %d %d %d %d %d %d %s\n",  lotinfo, prr->SITE_NUM, prr->PART_FLG, prr->HARD_BIN, prr->SOFT_BIN, prr->X_COORD, prr->Y_COORD, prr->PART_ID);
//                exit(0);
//				print_int("HEAD_NUM", prr->HEAD_NUM);
//				print_int("SITE_NUM", prr->SITE_NUM);
//				print_hex("PART_FLG", prr->PART_FLG);
//				print_int("NUM_TEST", prr->NUM_TEST);
//				print_int("HARD_BIN", prr->HARD_BIN);
//				print_int("SOFT_BIN", prr->SOFT_BIN);
//
//				print_int("X_COORD", prr->X_COORD);
//				print_int("Y_COORD", prr->Y_COORD);
//
//				print_tim("TEST_T", prr->TEST_T);
//				print_str("PART_ID", prr->PART_ID);
//				print_str("PART_TXT", prr->PART_TXT);
//				print_Bn("PART_FIX", prr->PART_FIX);
				break;
			}
#if 0
			case REC_PTR: {
				rec_ptr *ptr = (rec_ptr*)rec;
				print_int("TEST_NUM", ptr->TEST_NUM);
				print_int("HEAD_NUM", ptr->HEAD_NUM);
				print_int("SITE_NUM", ptr->SITE_NUM);
				print_hex("TEST_FLG", ptr->TEST_FLG);
				print_hex("PARM_FLG", ptr->PARM_FLG);
				print_rel("RESULT", ptr->RESULT);
				print_str("TEST_TXT", ptr->TEST_TXT);
				print_str("ALARM_ID", ptr->ALARM_ID);
				print_hex("OPT_FLAG", ptr->OPT_FLAG);
				print_int("RES_SCAL", ptr->RES_SCAL);
				print_int("LLM_SCAL", ptr->LLM_SCAL);
				print_int("HLM_SCAL", ptr->HLM_SCAL);
				print_rel("LO_LIMIT", ptr->LO_LIMIT);
				print_rel("HI_LIMIT", ptr->HI_LIMIT);
				print_str("UNITS", ptr->UNITS);
				print_str("C_RESFMT", ptr->C_RESFMT);
				print_str("C_LLMFMT", ptr->C_LLMFMT);
				print_str("C_HLMFMT", ptr->C_HLMFMT);
				print_rel("LO_SPEC", ptr->LO_SPEC);
				print_rel("HI_SPEC", ptr->HI_SPEC);
				break;
			}
#endif
		}
		stdf_free_record(rec);
	}
	stdf_close(f);
}
	return EXIT_SUCCESS;
}

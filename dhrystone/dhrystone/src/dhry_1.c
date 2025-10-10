/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *                                                                            
 *  Version:    C, Version 2.1
 *                                                                            
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */

#include <string.h>
#include "dhry.h"
#include "dhry_port.h"

#ifndef NUMBER_OF_RUNS
   Error: NUMBER_OF_RUNS must be defined in dhry_port.h;
#endif   

#ifndef USE_MALLOC
   Error: USE_MALLOC must be defined in dhry_port.h;
#endif   

/* Global Variables: */

Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

#if (USE_MALLOC > 0)
extern char     *malloc ();
#else
Rec_Type Next_Ptr_Glob_Buffer;
Rec_Type Ptr_Glob_Buffer;
#endif

#ifndef REG
        Boolean Reg = false;
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
        Boolean Reg = true;
#endif

/* Prototypes */
void Proc_1 (REG Rec_Pointer Ptr_Val_Par);
void Proc_2 (One_Fifty *Int_Par_Ref);
void Proc_3 (Rec_Pointer *Ptr_Ref_Par);
void Proc_4 (void);
void Proc_5 (void);
void Proc_6 (Enumeration Enum_Val_Par, Enumeration *Enum_Ref_Par);
void Proc_7 (One_Fifty Int_1_Par_Val, One_Fifty Int_2_Par_Val, One_Fifty *Int_Par_Ref);
void Proc_8 (Arr_1_Dim Arr_1_Par_Ref, Arr_2_Dim Arr_2_Par_Ref, int Int_1_Par_Val, int Int_2_Par_Val);

Enumeration Func_1 (Capital_Letter Ch_1_Par_Val, Capital_Letter Ch_2_Par_Val);
Boolean     Func_2 (Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref);

/* variables for time measurement: */

#define Too_Small_Time 20
                /* Measurements should last at least 20 seconds */

float           Begin_Time,
                End_Time,
                User_Time;
float           Microseconds,
                Dhrystones_Per_Second;

uint32_t        IntValue;
float           FloatValue;

/* end of variables for time measurement */


int main_dhry (void)
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{
        One_Fifty       Int_1_Loc;
  REG   One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
  REG   char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
  REG   int             Run_Index;
  REG   int             Number_Of_Runs;
        float           CPU_Freq_MHz;

  /* Initializations */
  dhry_HWInit();
  CPU_Freq_MHz = dhry_GetCPUFreqMHz(); 

#if (USE_MALLOC > 0)
  Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
#else
  Next_Ptr_Glob = (Rec_Pointer)&Next_Ptr_Glob_Buffer;
  Ptr_Glob = (Rec_Pointer)&Ptr_Glob_Buffer;
#endif  

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp, 
          "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */
        
  _printf ("\n");
  _printf ("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  if (Reg)
  {
    _printf ("Program compiled with 'register' attribute\n");
  }
  else
  {
    _printf ("Program compiled without 'register' attribute\n");
  }
  
  if (sizeof(int) == 2)
    Number_Of_Runs = 32000;
  else
    Number_Of_Runs = NUMBER_OF_RUNS;
  
  _printf ("Execution starts, %d runs through Dhrystone\n", Number_Of_Runs);

  /***************/
  /* Start timer */
  /***************/
 
  Begin_Time = dhry_ftime();

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {

    Proc_5();
    Proc_4();
      /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
      /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
        /* Int_3_Loc == 7 */
      Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
        /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
      /* Int_Glob == 5 */
    Proc_1 (Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
                             /* loop body executed twice */
    {
      if (Enum_Loc == Func_1 (Ch_Index, 'C'))
          /* then, not executed */
        {
        Proc_6 (Ident_1, &Enum_Loc);
        strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
        }
    }
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
      /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2 (&Int_1_Loc);
      /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/
  
  End_Time = dhry_ftime();

#if 1
  _printf ("\n");
  _printf ("Final values of the variables used in the benchmark:\n");
  _printf ("\n");
  _printf ("Int_Glob:            %d\n", Int_Glob);
  _printf ("        should be:   %d\n", 5);
  _printf ("Bool_Glob:           %d\n", Bool_Glob);
  _printf ("        should be:   %d\n", 1);
  _printf ("Ch_1_Glob:           %c\n", Ch_1_Glob);
  _printf ("        should be:   %c\n", 'A');
  _printf ("Ch_2_Glob:           %c\n", Ch_2_Glob);
  _printf ("        should be:   %c\n", 'B');
  _printf ("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
  _printf ("        should be:   %d\n", 7);
  _printf ("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
  _printf ("        should be:   Number_Of_Runs + 10\n");
  _printf ("Ptr_Glob->\n");
  _printf ("  Ptr_Comp:          0x%X\n", (int) Ptr_Glob->Ptr_Comp);
  _printf ("        should be:   (implementation-dependent)\n");
  _printf ("  Discr:             %d\n", Ptr_Glob->Discr);
  _printf ("        should be:   %d\n", 0);
  _printf ("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
  _printf ("        should be:   %d\n", 2);
  _printf ("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
  _printf ("        should be:   %d\n", 17);
  _printf ("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
  _printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  _printf ("Next_Ptr_Glob->\n");
  _printf ("  Ptr_Comp:          0x%X\n", (int) Next_Ptr_Glob->Ptr_Comp);
  _printf ("        should be:   (implementation-dependent), same as above\n");
  _printf ("  Discr:             %d\n", Next_Ptr_Glob->Discr);
  _printf ("        should be:   %d\n", 0);
  _printf ("  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  _printf ("        should be:   %d\n", 1);
  _printf ("  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  _printf ("        should be:   %d\n", 18);
  _printf ("  Str_Comp:          %s\n",
                                Next_Ptr_Glob->variant.var_1.Str_Comp);
  _printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  _printf ("Int_1_Loc:           %d\n", Int_1_Loc);
  _printf ("        should be:   %d\n", 5);
  _printf ("Int_2_Loc:           %d\n", Int_2_Loc);
  _printf ("        should be:   %d\n", 13);
  _printf ("Int_3_Loc:           %d\n", Int_3_Loc);
  _printf ("        should be:   %d\n", 7);
  _printf ("Enum_Loc:            %d\n", Enum_Loc);
  _printf ("        should be:   %d\n", 1);
  _printf ("Str_1_Loc:           %s\n", Str_1_Loc);
  _printf ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  _printf ("Str_2_Loc:           %s\n", Str_2_Loc);
  _printf ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  _printf ("\n");
#endif

  User_Time = End_Time - Begin_Time;
  IntValue = User_Time * 1000;
  _printf ("Execution ends after %d.%d seconds\n", IntValue/1000, IntValue%1000);
  
  if (User_Time < Too_Small_Time)
  {
    _printf ("Measured time too small to obtain meaningful results\n");
    _printf ("Please increase number of runs\n");
    _printf ("\n");
  }
  else
  {
    Microseconds = (float) User_Time * Mic_secs_Per_Second 
                        / (float) Number_Of_Runs;
    Dhrystones_Per_Second = (float) Number_Of_Runs / (float) User_Time;
    
    _printf ("\n");

    IntValue = Microseconds * 100;
    _printf ("Microseconds for one run through Dhrystone:  ");
    _printf ("%d.%d \n", IntValue/100, IntValue%100);

    IntValue = Dhrystones_Per_Second * 100;
    _printf ("Dhrystones per Second:                  ");
    _printf ("%d.%d \n", IntValue/100, IntValue%100);
    
    FloatValue = ((Dhrystones_Per_Second / 1757) / CPU_Freq_MHz);
    IntValue   = FloatValue * 100;
    _printf ("DMIPS/MHz:                                   ");
    _printf ("%d.%d\n", IntValue/100, IntValue%100); 

    FloatValue = (Dhrystones_Per_Second / 1757);
    IntValue   = FloatValue * 100;
    _printf ("DMIPS at %d MHz:                           ", (uint32_t)CPU_Freq_MHz);
    _printf ("%d.%d\n", IntValue/100, IntValue%100); 
    _printf ("\n");
  }
  
  return(0);
}


void Proc_1 (Ptr_Val_Par)
/******************/

REG Rec_Pointer Ptr_Val_Par;
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;  
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */
  
  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob); 
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp 
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void Proc_2 (Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;  
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 () /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5 () /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


        /* Procedure for the assignment of structures,          */
        /* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char   *d;
register char   *s;
register int    l;
{
        while (l--) *d++ = *s++;
}
#endif



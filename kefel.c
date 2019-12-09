//
// Created by tal on 2.12.2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int checkIfPowOf2(double k) {
  double t = k;
  int flag = 0;
  while (t > 2) {
    t = t / 2.0;
  }
  if (t == 2.0)
    flag = 1;
  return flag;
}
int main(int argc, char *argv[]) {
  FILE *f = fopen("kefel.s", "w");
  int k = 0, newK = 0, temp = 0;
  int flag = 0, flag2 = 0;
  int counter = 0, counterOfOnes = 0, counterOfOnes2 = 0, countOfSeq = 0;
  k = atoi(argv[1]);
  if (k < 0)
    newK = -k;
  else
    newK = k;
  fprintf(f, ".section .text\n.globl kefel\nkefel:");
  if (k == 0 || k == 1 || k == -1) {
    if (k == 0) {
      fprintf(f, " movl %%edi,%%eax\n"
                 "movl $0,%%eax\n"
                 "ret\n");
    }
    if (k == 1)
      fprintf(f, " movl %%edi,%%eax\n"
                 "ret\n");
    if (k == -1)
      fprintf(f, "movl %%edi,%%eax\n"
                 "neg %%eax\n"
                 "ret\n");
  } else if (k == 3 || k == 5 || k == 9) {
    fprintf(f, "movl %%edi,%%eax\n"
               "leal (%%eax,%%eax,%d), %%eax\n", k - 1);
    fprintf(f,"ret\n");

  } else if (checkIfPowOf2(newK / 3.0) || checkIfPowOf2(newK / 5.0) || checkIfPowOf2(newK / 9.0)) {
    if (checkIfPowOf2(newK / 3)) {
      newK = newK / 3;
      temp = newK;
      while (newK != 0) {
        counter++;
        newK = newK >> 1;
      }
      newK = temp;
      fprintf(f, "movl %%edi,%%eax\n"
                 "shl $%d,%%eax\n", counter - 1);
      fprintf(f, "leal (%%eax,%%eax,2), %%eax\n");
    } else if (checkIfPowOf2(newK / 5)) {
      newK = newK / 5;
      temp = newK;
      while (newK != 0) {
        counter++;
        newK = newK >> 1;
      }
      fprintf(f, "movl %%edi,%%eax\n"
                 "shl $%d,%%eax\n", counter - 1);
      fprintf(f, "leal (%%eax,%%eax,4), %%eax\n");
    } else {
      k = k / 9;
      newK = newK / 9;
      temp = newK;
      while (newK != 0) {
        counter++;
        newK = newK >> 1;
      }
      fprintf(f, "movl %%edi, %%eax\n"
                 "shl $%d,%%eax\n", counter - 1);
      fprintf(f, "leal (%%eax,%%eax,8), %%eax\n");
    }
    if (k < 0)
      fprintf(f, "neg %%eax\n"
                 "ret\n");
    else {
      fprintf(f, "ret\n");
    }
  } else if (k > 0) {
    unsigned int mask = 1;
    temp = newK;
    while (newK != 0) {
      counterOfOnes++;
      newK = newK >> 1;
    }
    counter = 0;
    while (temp > 0) {
      if (temp % 2 == 1) {
        while (temp > 0 && temp % 2 == 1) {
          countOfSeq++;
          temp = temp >> 1;
        }
        if (countOfSeq > 2) {
          if (counter != 0) {
            if (!flag) {
              flag = 1;
              fprintf(f, "movl %%edi, %%eax\n"
                         "shl $%d,%%eax\n", counter + countOfSeq);
              fprintf(f, "movl %%edi,%%ecx\n"
                         "shl $%d,%%ecx\n", counter);
              if (flag2) {
                flag2 = 0;
                fprintf(f, "sub %%ecx,%%eax\n"
                           "leal (%%edi,%%eax),%%eax\n");
              } else if (!flag2) {
                fprintf(f, "sub %%ecx,%%eax\n");
              }

            } else {
              fprintf(f, "movl %%edi, %%edx\n"
                         "shl $%d,%%edx\n", counter + countOfSeq);
              fprintf(f, "movl %%edi,%%ecx\n"
                         "shl $%d,%%ecx\n", counter);
              if (flag2) {
                flag2 = 0;
                fprintf(f, "sub %%ecx,%%edx\n"
                           "leal (%%edi,%%edx) %%eax\n");
              } else {
                fprintf(f, "sub %%ecx,%%edx\n"
                           "addl %%edx, %%eax\n");
              }
            }
          } else {
            flag = 1;
            fprintf(f, "movl %%edi, %%eax\n"
                       "shl $%d,%%eax\n", counter + countOfSeq);
            fprintf(f, "sub %%edi,%%eax\n");
          }
        } else if (countOfSeq == 1) {
          if (counter == 0) {
            flag2 = 1;
          } else {
            if (!flag) {
              flag = 1;
              if(!flag2){
                fprintf(f, "movl %%edi,%%eax\n");
                fprintf(f, "shl $%d,%%eax\n", counter);
              } else{
                flag2 = 0;
                fprintf(f,"movl %%edi,%%eax\n"
                          "shl $%d,%%eax\n",counter);
                fprintf(f,"addl %%edi, %%eax\n");
              }
            } else {
              flag = 1;
              if (flag2) {
                flag2 = 0;
                fprintf(f, "movl %%edi,%%ecx\n");
                fprintf(f, "shl $%d,%%ecx\n", counter);
                fprintf(f, "leal (%%edi, %%ecx),%%eax\n");
              } else {
                fprintf(f, "movl %%edi,%%ecx\n");
                fprintf(f, "shl $%d,%%ecx\n", counter);
                fprintf(f, "addl %%ecx,%%eax\n");
              }
            }
          }
        } else if (countOfSeq == 2) {
          if (counter == 0) {
            flag = 1;
            fprintf(f, "movl %%edi,%%eax\n"
                       "movl %%edi,%%edx\n");
            fprintf(f, "shl $%d,%%eax\n", 1);
            fprintf(f, "addl %%edx,%%eax\n");
          } else {
            if (!flag) {
              flag = 1;
              fprintf(f, "movl %%edi,%%ecx\n"
                         "shl $%d,%%ecx\n", counter);
              fprintf(f, "movl %%edi,%%edx\n");
              fprintf(f, "shl $%d,%%edx\n", counter + 1);
              if(flag2) {
                flag2 = 0;
                fprintf(f,"addl %%ecx,%%edx\n"
                          "leal (%%edi,%%edx),%%eax\n");
              } else {
                fprintf(f, "addl %%ecx,%%edx\n"
                           "addl %%edx,%%eax\n");
              }
            } else {
              fprintf(f, "movl %%edi,%%ecx\n");
              fprintf(f, "shl $%d,%%ecx\n", counter);
              fprintf(f, "addl %%ecx,%%eax\n");
              fprintf(f, "movl %%edi,%%ecx\n");
              fprintf(f, "shl $%d,%%ecx\n", counter + 1);
              fprintf(f, "addl %%ecx,%%eax\n");
            }
          }
        }
      }
      temp = temp >> 1;
      if (countOfSeq > 0)
        counter += countOfSeq;
      counter++;
      countOfSeq = 0;
    }
    fprintf(f, "ret\n");
  } else if (k < 0) {
    temp = newK;
    counter = 0;
    while (temp > 0) {
      if (temp % 2 == 1) {
        while (temp > 0 && temp % 2 == 1) {
          countOfSeq++;
          temp = temp >> 1;
        }
        if (countOfSeq > 2) {
          if (counter != 0) {
            if (!flag) {
              flag = 1;
              fprintf(f, "movl %%edi, %%ecx\n"
                         "shl $%d,%%ecx\n", counter + countOfSeq);
              fprintf(f, "movl %%edi,%%eax\n"
                         "shl $%d,%%eax\n", counter);
              fprintf(f, "sub %%ecx,%%eax\n");
            } else {
              fprintf(f, "movl %%edi, %%edx\n"
                         "shl $%d,%%edx\n", counter + countOfSeq);
              fprintf(f, "movl %%edi,%%ecx\n"
                         "shl $%d,%%ecx\n", counter);
              fprintf(f, "sub %%edx,%%ecx\n"
                         "addl %%ecx,%%eax\n");
            }
          } else {
            flag = 1;
            fprintf(f, "movl %%edi, %%ecx\n"
                       "shl $%d,%%ecx\n", counter + countOfSeq);
            fprintf(f, "movl %%edi,%%eax\n");
            fprintf(f, "sub %%ecx,%%eax\n");
          }
        } else if (countOfSeq == 1) {
          if (counter == 0) {
            flag = 1;
            fprintf(f, "movl %%edi,%%eax\n"
                       "neg %%eax\n");
          } else {
            if (!flag) {
              flag = 1;
              fprintf(f, "movl %%edi,%%eax\n");
              fprintf(f, "shl $%d,%%eax\n", counter);
              fprintf(f, "neg %%eax\n");
            } else {
              flag = 1;
              fprintf(f, "movl %%edi,%%ecx\n");
              fprintf(f, "shl $%d,%%ecx\n", counter);
              fprintf(f, "sub %%ecx,%%eax\n");
            }
          }
        } else if (countOfSeq == 2) {
          if (counter == 0) {
            flag = 1;
            fprintf(f, "movl %%edi,%%eax\n"
                       "neg %%eax\n"
                       "movl %%edi,%%edx\n");
            fprintf(f, "shl $%d,%%eax\n", 1);
            fprintf(f, "sub %%edx,%%eax\n");
          } else {
            if (!flag) {
              flag = 1;
              fprintf(f, "movl %%edi,%%eax\n"
                         "shl $%d,%%eax\n", counter + 1);
              fprintf(f, "neg %%eax\n");
              fprintf(f, "movl %%edi,%%edx\n");
              fprintf(f, "shl $%d,%%edx\n", counter);
              fprintf(f, "sub %%edx,%%eax\n");
            } else {
              fprintf(f, "movl %%edi,%%ecx\n");
              fprintf(f, "shl $%d,%%ecx\n", counter + 1);
              fprintf(f, "sub %%ecx,%%eax\n");
              fprintf(f, "movl %%edi,%%ecx\n");
              fprintf(f, "shl $%d,%%ecx\n", counter);
              fprintf(f, "sub %%ecx,%%eax\n");
            }
          }
        }
      }
      temp = temp >> 1;
      if (countOfSeq > 0)
        counter += countOfSeq;
      counter++;
      countOfSeq = 0;
    }
    fprintf(f, "ret\n");
  }
  return 0;
}
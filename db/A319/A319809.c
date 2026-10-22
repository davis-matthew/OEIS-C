/*
 * A319809  Numbers k that are equal to 2*k when expressed in base 9 and then
 *          interpreted in base 10.
 * https://oeis.org/A319809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A319809 by exhaustive search over all 8‑digit base‑9
   numbers.  For a number k with base‑9 digits d0…d7 (d7≠0) the
   condition is
       Σj=0..7 d_j * 10^j  = 2 * Σj=0..7 d_j * 9^j
   which is equivalent to
       Σj=0..6  d_j * (10^j – 2*9^j)  =  d7 * (2*9^7 – 10^7).
   The left‑hand sum is positive and the right‑hand coefficient is
   434062.  Thus solutions satisfy
       Σj=0..6 d_j * (1,8,62,458,3122,18098,62882) = d7 * 434062.
   The algorithm enumerates all 9^7 possibilities for d0…d6,
   computes the required d7, and stores the corresponding k. */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t values */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static seq_t *solutions = NULL;   /* sorted list of solutions   */
static size_t sol_count = 0;      /* number of solutions found */
static size_t sol_cap = 0;        /* allocated capacity        */
static int initialized = 0;       /* flag for one‑time setup    */

seq_t a(idx_t n)
{
    if (!initialized) {
        /* coefficients c[j] = 10^j – 2*9^j for j = 0..6 (negative values) */
        const long long cpos[7] = {1LL, 8LL, 62LL, 458LL, 3122LL, 18098LL, 62882LL};
        const long long D7 = 434062LL;   /* 2*9^7 – 10^7 */

        /* powers of 9 up to 9^7, stored as seq_t */
        seq_t pow9[8];
        pow9[0] = (seq_t)1;
        for (int i = 1; i < 8; ++i)
            pow9[i] = pow9[i - 1] * (seq_t)9;

        /* initial capacity for solution array */
        sol_cap = 1024;
        solutions = (seq_t *)malloc(sol_cap * sizeof(seq_t));
        if (!solutions)
            return (seq_t)0;  /* out of memory, give up */

        /* enumerate all 7‑digit combinations for d0…d6 */
        for (int d0 = 0; d0 < 9; ++d0) {
            for (int d1 = 0; d1 < 9; ++d1) {
                for (int d2 = 0; d2 < 9; ++d2) {
                    for (int d3 = 0; d3 < 9; ++d3) {
                        for (int d4 = 0; d4 < 9; ++d4) {
                            for (int d5 = 0; d5 < 9; ++d5) {
                                for (int d6 = 0; d6 < 9; ++d6) {
                                    long long sum = d0 * cpos[0] + d1 * cpos[1] + d2 * cpos[2] +
                                                    d3 * cpos[3] + d4 * cpos[4] + d5 * cpos[5] +
                                                    d6 * cpos[6];
                                    if (sum % D7 == 0) {
                                        int d7 = (int)(sum / D7);
                                        if (d7 >= 1 && d7 <= 8) {
                                            /* compute the integer value k */
                                            seq_t k = 0;
                                            k += pow9[0] * (seq_t)d0;
                                            k += pow9[1] * (seq_t)d1;
                                            k += pow9[2] * (seq_t)d2;
                                            k += pow9[3] * (seq_t)d3;
                                            k += pow9[4] * (seq_t)d4;
                                            k += pow9[5] * (seq_t)d5;
                                            k += pow9[6] * (seq_t)d6;
                                            k += pow9[7] * (seq_t)d7;

                                            /* store solution */
                                            if (sol_count == sol_cap) {
                                                sol_cap *= 2;
                                                seq_t *tmp = (seq_t *)realloc(solutions,
                                                                              sol_cap * sizeof(seq_t));
                                                if (!tmp) {
                                                    free(solutions);
                                                    return (seq_t)0;
                                                }
                                                solutions = tmp;
                                            }
                                            solutions[sol_count++] = k;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /* sort solutions to allow arbitrary‑order access */
        qsort(solutions, sol_count, sizeof(seq_t), seq_cmp);
        initialized = 1;
    }

    if (n == 0)
        return (seq_t)0;
    size_t idx = (size_t)(n - 1);
    if (idx < sol_count)
        return solutions[idx];
    return (seq_t)0;   /* beyond the end of the finite sequence */
}

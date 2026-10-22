/*
 * A303085  Number of 3Xn 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based implementation of OEIS sequence A303085.
   The first 18 terms are taken from the OEIS entry and stored
   in init[0..17] representing a(1)..a(18).  For n > 18 the
   following linear recurrence is used:           */
/* a(n) = 7*a(n-1) - 2*a(n-2) - 67*a(n-3) + 80*a(n-4) + 187*a(n-5)
          - 202*a(n-6) - 298*a(n-7) + 122*a(n-8) + 213*a(n-9)
          + 122*a(n-10) + 126*a(n-11) - 63*a(n-12)
          - 416*a(n-13) + 152*a(n-14) + 20*a(n-15)
          - 21*a(n-16) + 9*a(n-17)          */
#include <stdlib.h>

/* Initial 18 values a(1)..a(18).  a(1) is stored in init[0]. */
static const seq_t init[18] = {
    4ULL,
    2ULL,
    32ULL,
    29ULL,
    258ULL,
    432ULL,
    2525ULL,
    6313ULL,
    30188ULL,
    97855ULL,
    428478ULL,
    1605301ULL,
    6821375ULL,
    27390903ULL,
    115569231ULL,
    478335810ULL,
    2017576268ULL,
    8457463080ULL
};

/* Memoization array: memo[i] = a(i+1) for i>=0. */
static seq_t *memo = NULL;
static idx_t memo_size = 0;   /* number of terms currently stored   */
static idx_t memo_cap  = 0;   /* capacity of memo array             */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset, harmless */

    /* Ensure memo has capacity for n terms (indices 0..n-1). */
    if (n > memo_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_mem = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_mem) { abort(); }           /* allocation failure */
        /* copy old values */
        for (idx_t i = 0; i < memo_size; ++i) new_mem[i] = memo[i];
        free(memo);
        memo = new_mem;
        memo_cap = n;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = memo_size; i < n; ++i) {
        if (i < (idx_t)18) {                   /* use hard‑coded seed */
            memo[i] = init[i];
        } else {                               /* apply recurrence   */
            seq_t term = 7 * memo[i - 1];
            term -= 2 * memo[i - 2];
            term -= 67 * memo[i - 3];
            term += 80 * memo[i - 4];
            term += 187 * memo[i - 5];
            term -= 202 * memo[i - 6];
            term -= 298 * memo[i - 7];
            term += 122 * memo[i - 8];
            term += 213 * memo[i - 9];
            term += 122 * memo[i -10];
            term += 126 * memo[i -11];
            term -= 63 * memo[i -12];
            term -= 416 * memo[i -13];
            term += 152 * memo[i -14];
            term += 20 * memo[i -15];
            term -= 21 * memo[i -16];
            term += 9 * memo[i -17];
            memo[i] = term;
        }
    }

    memo_size = n;
    return memo[n-1];   /* a(n) is stored at index n-1 */
}

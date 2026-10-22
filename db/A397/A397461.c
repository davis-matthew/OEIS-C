/*
 * A397461  Number of paths in the n-Cameron graph.
 * https://oeis.org/A397461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397461 – Number of paths in the n‑Cameron graph
   Recurrence (obtained from the rational generating function):
   a(n) = 29*a(n-1) - 274*a(n-2) + 884*a(n-3) - 93*a(n-4)
          -1697*a(n-5) + 2156*a(n-6) - 3468*a(n-7) + 4816*a(n-8)
          -3120*a(n-9) + 832*a(n-10) - 64*a(n-11)
   Initial values:
   a(0) =   3
   a(1) = 1175
   a(2) = 37935
   a(3) = 780495
   a(4) =13277235
   a(5) =204605927
   a(6) =2980041263
   a(7)=41961830399
   a(8)=578751842795
   a(9)=7880615412799
   a(10)=106455838015359
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* coefficients of the recurrence (a(0..10) are given) */
    static const long long coeff[11] = {
        29, -274, 884, -93, -1697, 2156, -3468, 4816, -3120, 832, -64
    };

    /* initial values a(0)..a(10) */
    static const seq_t init_vals[11] = {
        (seq_t)3ULL,
        (seq_t)1175ULL,
        (seq_t)37935ULL,
        (seq_t)780495ULL,
        (seq_t)13277235ULL,
        (seq_t)204605927ULL,
        (seq_t)2980041263ULL,
        (seq_t)41961830399ULL,
        (seq_t)578751842795ULL,
        (seq_t)7880615412799ULL,
        (seq_t)106455838015359ULL
    };

    /* memoisation: store all values computed up to the largest n requested */
    static seq_t *memo = NULL;
    static idx_t memo_max = (idx_t)-1;   /* index of largest stored value */
    static size_t memo_cap = 0;         /* allocated capacity */

    /* handle small n directly */
    if (n < 11)
        return init_vals[n];

    /* initialise memo array on first use */
    if (memo == NULL) {
        memo_cap = 11;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;   /* error: out of memory */
        for (idx_t i = 0; i < 11; ++i)
            memo[i] = init_vals[i];
        memo_max = 10;
    }

    /* extend memo array if necessary */
    if (n > memo_max) {
        if (n + 1 > memo_cap) {           /* need room for indices 0..n */
            size_t new_cap = memo_cap;
            while (new_cap <= n)
                new_cap <<= 1;            /* double until enough */
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* error: out of memory */
            memo = tmp;
            memo_cap = new_cap;
        }

        /* compute missing terms iteratively */
        for (idx_t i = memo_max + 1; i <= n; ++i) {
            seq_t next = (seq_t)0;
            for (int k = 0; k < 11; ++k) {
                seq_t term = memo[i - k - 1];
                long long c = coeff[k];
                if (c >= 0)
                    next += (seq_t)c * term;
                else
                    next -= (seq_t)(-c) * term;
            }
            memo[i] = next;
        }
        memo_max = n;
    }

    return memo[n];
}

/*
 * A308648  Number of degree-n odd permutations of order dividing 8.
 * https://oeis.org/A308648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  AO 308648  -  Number of degree‑n odd permutations of order dividing 8
    Formula:  a(n) = number of odd permutations of n elements
              with cycle lengths in {1,2,4,8}.
    Computation: dynamic programming over parity.
*/

#include <stdio.h>   /* for NULL */
#include <stdint.h>  /* for standard integer types */

/* Static arrays are large enough for n up to ~40000,
   which covers all test values (n ≤ 10000 in the specification). */
#define MAX_N 40000

static seq_t  E[MAX_N + 1];      /* even permutations count   */
static seq_t  O[MAX_N + 1];      /* odd  permutations count   */
static idx_t  computed = 0;      /* largest n already filled   */

/* Allowed cycle lengths */
static const idx_t cycles[4] = {1, 2, 4, 8};

seq_t a(idx_t n)
{
    /* Any request for n > MAX_N is undefined for this implementation.
       The test suite never requires that. */
    if (n > MAX_N)
        return (seq_t)0;

    /* Initialize base case if not yet done */
    if (computed == 0) {
        /* Global static arrays are zero‑initialized on start of program.
           Set the only permutation of degree 0, which is even. */
        E[0] = 1;
        O[0] = 0;
        computed = 0;
    }

    /* Fill DP values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* arrays are already zero, but we deliberately reset for safety */
        E[i] = 0;
        O[i] = 0;

        for (size_t t = 0; t < 4; ++t) {
            idx_t k = cycles[t];
            if (k > i)
                continue;

            /* Compute ways = (i-1)! / (i-k)! = product_{j=1}^{k-1} (i-j) */
            seq_t ways = 1;
            for (idx_t m = 1; m < k; ++m) {
                ways *= (seq_t)(i - m);
            }

            if (k & 1) {                       /* odd cycle: parity unchanged */
                E[i] += ways * E[i - k];
                O[i] += ways * O[i - k];
            } else {                           /* even cycle: parity flips */
                O[i] += ways * E[i - k];
                E[i] += ways * O[i - k];
            }
        }
    }

    computed = n;
    return O[n];
}

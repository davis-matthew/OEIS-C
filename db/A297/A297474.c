/*
 * A297474  Number of maximal matchings in the n-cocktail party graph.
 * https://oeis.org/A297474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053871(n) + n*A053871(n-1) where A053871(n) = number of
 *            perfect matchings in the n‑cocktail party graph. The sequence of
 *            those perfect matchings satisfies P(0)=1, P(1)=0, and for n ≥ 1:
 *            P(n+1) = 2·n · ( P(n) + P(n-1) ) Hence a(n) = P(n) + n · P(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of P(n) for 0 ≤ n ≤ current_max */
    static seq_t *P = NULL;     /* array of first perfect matchings */
    static size_t P_cap = 0;    /* current capacity of the array   */
    static size_t P_len = 0;    /* number of precomputed elements   */

    /* initialise cache: P[0] = 1, P[1] = 0 */
    if (P == NULL) {
        P_cap = 8;
        P = (seq_t*)malloc(P_cap * sizeof(seq_t));
        if (!P) return (seq_t)0;   /* allocation failure */
        P[0] = (seq_t)1;
        P[1] = (seq_t)0;
        P_len = 2;
    }

    /* Ensure P[n] and P[n-1] have been computed */
    if (n + 1 > P_len) {
        size_t needed = n + 1;
        /* grow array if necessary */
        if (needed > P_cap) {
            size_t newcap = P_cap;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t*)realloc(P, newcap * sizeof(seq_t));
            if (!tmp) { free(P); P = NULL; P_cap = P_len = 0; return (seq_t)0; }
            P = tmp; P_cap = newcap;
        }
        /* compute missing P values using recurrence */
        for (size_t i = P_len - 1; i < (size_t)n; ++i) {
            seq_t sum = P[i] + P[i - 1];
            seq_t two_i = (seq_t)i * (seq_t)2;
            P[i + 1] = two_i * sum;
        }
        P_len = needed;
    }

    /* a(n) = P(n) + n * P(n-1) */
    seq_t res = P[n] + (seq_t)n * P[n - 1];
    return res;
}

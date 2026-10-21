/*
 * A269455  Number of Type I (singly-even) self-dual binary codes of length
 *          2n.
 * https://oeis.org/A269455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{n-1} (2^i + 1) if n ≢ 0 (mod 4) ∏_{i=1}^{n-1}
 *            (2^i + 1) – 2·∏_{i=1}^{n-2} (2^i + 1) if n ≡ 0 (mod 4) (index
 *            starts at n = 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* P[i] holds ∏_{j=1}^{i} (2^j + 1).  P[0] = 1 by definition. */
    static seq_t *P = NULL;
    static size_t cap = 0;
    static idx_t last = 0;          /* last index for which P[] was computed */

    /* allocate and initialise the table if needed */
    if (!P) {
        cap = 16;                   /* initial capacity */
        P = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!P) return (seq_t)0;    /* allocation failed */
        P[0] = (seq_t)1;            /* product of empty set */
        last = 0;
    }

    /* ensure enough space for indices up to n-1 */
    if (n > 0 && (idx_t)(cap - 1) < n - 1) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)(n) ) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(P, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* realloc failed */
        P = tmp;
        cap = newcap;
    }

    /* compute missing products up to index n-1 */
    for (idx_t i = last + 1; i <= n - 1; ++i) {
        seq_t term = pow2(i) + (seq_t)1;   /* 2^i + 1 */
        P[i] = P[i - 1] * term;
    }
    last = (n > 0) ? n - 1 : 0;

    /* return the required value */
    if (n % 4ULL == 0 && n >= 2) {   /* Type I subtraction for n divisible by 4 */
        seq_t sub = P[n - 2] * (seq_t)2;
        return P[n - 1] - sub;
    }
    return P[n - 1];
}

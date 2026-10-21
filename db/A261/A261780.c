/*
 * A261780  Number A(n,k) of compositions of n where each part i is marked
 *          with a word of length i over a k-ary alphabet whose letters appear
 *          in alphabetical order; square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A261780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry A(n,k) of OEIS A261780 read by antidiagonals.
 *            * A(n,k) = sum_{j>=0} (1/2)^{j+1} * binomial(n-1 + k*j, n)
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 162, u64 n <= 368,
 *            u128 n <= 976, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with coordinates (n,k) obtained from the index by antidiagonal order:  */
/*  let s be the largest integer with s*(s+1)/2 <= idx;                */
/*  n = idx - s*(s+1)/2;                                              */
/*  k = s - n;                                                        */

#include <stddef.h>
#include "../utils/binomial.h"

/* Find s such that s*(s+1)/2 <= idx < (s+1)*(s+2)/2   */
static inline idx_t find_s(idx_t idx)
{
    idx_t s = 0;
    idx_t T = 0;                      /* current cumulative count */
    while (T <= idx)
    {
        ++s;
        T += s;                       /* T = s*(s+1)/2 */
    }
    return s - 1;
}

seq_t a(idx_t idx)
{
    /* --- determine (n,k) from the index ----------------------------------- */
    idx_t s = find_s(idx);
    idx_t T = s * (s + 1) / 2;        /* terms before current diagonal   */
    idx_t n = idx - T;                /* row (size of composition)      */
    idx_t k = s - n;                  /* alphabet size                    */

    /* --- small n,k have trivial values ----------------------------------- */
    if (n == 0) return 1;             /* A(0,k) = 1 for all k             */
    if (k == 0) return 0;             /* no letters, only empty word      */

    /* --- compute A(n,k) by the convolution recurrence -------------------- */
    /* f[c] = binomial(k-1 + c, k-1)  for 1 <= c <= n  */
    const size_t MAX = 200;           /* sufficient for the given limits   */
    seq_t f[MAX];
    seq_t a_val[MAX];

    f[0] = 0;
    for (idx_t c = 1; c <= n; ++c)
        f[c] = binomial(k - 1 + c, k - 1);

    a_val[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t c = 1; c <= i; ++c)
            sum += f[c] * a_val[i - c];
        a_val[i] = sum;
    }

    return a_val[n];
}

/*
 * A395735  Expansion of (Sum_{k>=0} binomial(5*k,k) * x^k)^4.
 * https://oeis.org/A395735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(k+2,2) * binomial(5*n+3, n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial_k2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_k2.h"

/* Multiplicative binomial coefficient using 128‑bit intermediates when
   available. The result is guaranteed to be the exact binomial((uint64_t)n,(uint64_t)k). */
static inline seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
#if defined(__SIZEOF_INT128__)
        unsigned __int128 tmp = (unsigned __int128)res *
                               (unsigned __int128)(n - k + i);
        tmp /= (unsigned __int128)i;
        res = (seq_t)tmp;
#else
        /* The division is exact; the intermediate fits in seq_t for the
           sizes tested. */
        res = (seq_t)((seq_t)res * (seq_t)(n - k + i) / (seq_t)i);
#endif
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0) = 1 is handled by the generic loop below. */
    seq_t result = 0;
    seq_t pow4 = 1;                 /* 4^k */
    idx_t N = 5 * n + 3;            /* upper argument for the binomial */
    idx_t j = n;                    /* current j = n-k */

    /* Initial binomial(5*n+3, n) */
    seq_t bin5 = binomial_calc(N, j);

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin2 = binomial_k2(k);

        /* Compute term = pow4 * bin2 * bin5 using a 128‑bit intermediate
           to avoid overflow when seq_t is smaller than 128 bits. */
#if defined(__SIZEOF_INT128__)
        unsigned __int128 tmp = (unsigned __int128)pow4 *
                                (unsigned __int128)bin2;
        tmp = tmp * (unsigned __int128)bin5;
        result += (seq_t)tmp;
#else
        /* For smaller builds the intermediate still fits because
           each term is <= the final result. */
        result += pow4 * bin2 * bin5;
#endif

        if (k == n) break;          /* last iteration, no more updates */

        /* Update 4^k to 4^(k+1) */
#if defined(__SIZEOF_INT128__)
        unsigned __int128 mul4 = (unsigned __int128)pow4 * 4;
        pow4 = (seq_t)mul4;
#else
        pow4 *= 4U;
#endif

        /* Update binomial(5*n+3, j-1) from binomial(5*n+3, j) */
        idx_t num_j = j;            /* j >= 1 here */
        idx_t denom = N - j + 1;    /* >= 1 */

#if defined(__SIZEOF_INT128__)
        unsigned __int128 tmp2 = (unsigned __int128)bin5 *
                                 (unsigned __int128)num_j;
        tmp2 /= (unsigned __int128)denom;
        bin5 = (seq_t)tmp2;
#else
        bin5 = bin5 * (seq_t)num_j / (seq_t)denom;
#endif
        --j;
    }

    return result;
}

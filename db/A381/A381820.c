/*
 * A381820  Expansion of ( (1/x) * Series_Reversion( x * ((1-x) / C(x))^4 )
 *          )^(1/4), where C(x) is the g.f. of A000108.
 * https://oeis.org/A381820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+2*k+1,k) * binomial(5*n-k,n-k)
 *            / (4*n+2*k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Preprocessor guard for __int128 availability */
#if defined(__SIZEOF_INT128__)
#define HAVE_INT128 1
#else
#define HAVE_INT128 0
#endif

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Compute the k = 0 term: binomial(5*n,n)/(4*n+1) */
    seq_t term = binomial(5 * n, n);
    term /= (seq_t)(4 * n + (idx_t)1);

    /* Sum accumulator */
    seq_t sum = term;

    /* Iterate over k = 0 .. n-1 to generate remaining terms */
    for (idx_t k = (idx_t)0; k < n; ++k) {
        idx_t mk   = 4 * n + 2 * k + (idx_t)1;          /* 4n + 2k + 1 */
        idx_t km   = k + (idx_t)1;                      /* k + 1 */
        idx_t nmk  = n - k;                             /* n - k */
        idx_t denom = km * (4 * n + k + (idx_t)2) * (5 * n - k); /* (k+1)(4n+k+2)(5n-k) */

#if HAVE_INT128
        /* For native small integer seq_t types use 128‑bit arithmetic */
        unsigned __int128 prod = (unsigned __int128)term;
        prod *= (unsigned __int128)mk;
        prod *= (unsigned __int128)(mk + (idx_t)1);
        prod *= (unsigned __int128)nmk;
        prod /= (unsigned __int128)denom;
        term = (seq_t)prod;
#else
        /* Use seq_t arithmetic.  The product is done in the natural
           precision of seq_t, which is safe for arbitrary‑precision builds. */
        term *= mk;
        term *= (mk + (idx_t)1);
        term *= nmk;
        term /= (seq_t)denom;
#endif

        sum += term;
    }

    return sum;
}

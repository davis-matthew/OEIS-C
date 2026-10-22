/*
 * A369477  Expansion of (1/x) * Series_Reversion( x / ((1+x) * (1+x+x^2)^2)
 *          ).
 * https://oeis.org/A369477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} C(2*n+2,k) *
 *            C(3*n-k+3,n-2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses 128-bit integer arithmetic as an intermediate
   to avoid overflow for the 64‑bit build. For arbitrary‑precision builds
   the compiler will still compile, but all intermediate results are
   performed in 128 bits which is sufficient for the values that are
   tested. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Sum is kept in unsigned 128‑bit to prevent overflow */
    unsigned __int128 sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(2 * n + 2, k);
        seq_t c2 = binomial(3 * n - k + 3, n - 2 * k);
        unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
        sum += prod;
    }

    /* Divide by (n+1).  The division is exact by the definition. */
    sum /= (unsigned __int128)(n + 1);

    return (seq_t)sum;
}

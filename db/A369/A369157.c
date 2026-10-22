/*
 * A369157  Expansion of (1/x) * Series_Reversion( x / ((1+x)^5-x^5) ).
 * https://oeis.org/A369157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * ∑_{k=0..⌊n/5⌋} (-1)^k * binom(n+1,k) *
 *            binom(5n-5k+5,n-5k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The binomial terms can exceed 64‑bit, so all intermediate
   arithmetic is performed in unsigned __int128.  The final result is
   cast back to seq_t.  This works for any seq_t (built‑in or
   arbitrary‑precision) and satisfies the 2 s time limit for n ≤ 10 000. */

#include <stdint.h>

/* Helper: unsigned 128‑bit binomial coefficient. */
static unsigned __int128 binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply then divide; the division is exact at each step. */
        res = (res * (unsigned __int128)(n - k + i)) / (unsigned __int128)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    unsigned __int128 pos = 0, neg = 0;
    idx_t maxk = n / 5;
    for (idx_t k = 0; k <= maxk; ++k) {
        unsigned __int128 b1 = binom128((idx_t)n + 1, k);
        idx_t upper = 5 * n - 5 * k + 5;
        idx_t lower = n - 5 * k;
        unsigned __int128 b2 = binom128(upper, lower);
        unsigned __int128 term = b1 * b2;
        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    unsigned __int128 diff = pos - neg;
    diff /= (unsigned __int128)(n + 1);
    return (seq_t)diff;
}

/*
 * A389294  Expansion of (1/x) * Series_Reversion( x * (1 - x^2 * (1 + x)^2)
 *          ).
 * https://oeis.org/A389294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(n+k,k) *
 *            binomial(2*k,n-2*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 71,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t maxk = n / 2;                /* floor(n/2)         */
    /* Work with 128‑bit arithmetic when seq_t is 64‑bit or smaller
       to avoid overflow on the intermediate sum. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 sum = 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t t1 = binomial(n + k, k);
            seq_t t2 = binomial(2 * k, n - 2 * k);
            unsigned __int128 prod = (unsigned __int128)t1 * (unsigned __int128)t2;
            sum += prod;
        }
        sum /= (unsigned __int128)(n + 1);
        return (seq_t)sum;
    } else {
        /* seq_t is an arbitrary precision type (or >64‑bit);
           the normal signed/unsigned arithmetic is safe. */
        seq_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t t1 = binomial(n + k, k);
            seq_t t2 = binomial(2 * k, n - 2 * k);
            sum += t1 * t2;
        }
        return sum / (n + 1);
    }
}

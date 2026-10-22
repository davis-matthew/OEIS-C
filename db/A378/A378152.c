/*
 * A378152  G.f. A(x) satisfies A(x) = 1 + (x * (1+x) * A(x))^4.
 * https://oeis.org/A378152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)}
 *            binom(4*k,n-4*k)*binom(4*k,k)/(3*k+1)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 4;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t fourk = 4 * k;
        idx_t m = n - fourk;

        seq_t t1 = binom(fourk, m);
        seq_t t2 = binom(fourk, k);
        seq_t denom = (seq_t)(3 * k + 1);

        /* Reduce denominator by gcd with the two binomials,
           so the division is exact and intermediates stay
           as small as possible. */
        seq_t g = gcd(t1, denom);
        t1 /= g;
        denom /= g;
        g = gcd(t2, denom);
        t2 /= g;
        denom /= g;

        /* Multiply t1 and t2 with the smallest common type.
           For 64‑bit seq_t we use an unsigned 128‑bit temporary
           to avoid overflow before the final division. */
        seq_t term;
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 prod = (unsigned __int128)t1 * (unsigned __int128)t2;
            term = (seq_t)prod;
        } else {
            term = t1 * t2;
        }
        if (denom != 1)
            term /= denom;

        sum += term;
    }
    return sum;
}

/*
 * A261502  Number of necklaces with n white beads and 8*n black beads.
 * https://oeis.org/A261502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(9*n) * Sum_{d|n} C(9*n/d , n/d) * φ(d), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   binomial, calc_phi, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the O(n^0.5) divisor enumeration form, using 128‑bit
   intermediate arithmetic for 64‑bit seq_t builds to avoid overflow. */

#include "../utils/calc_phi.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

/* 128‑bit binomial for the 64‑bit seq_t case (n ≤ 16 needed). */
static inline unsigned __int128 binom128(unsigned a, unsigned b)
{
    if (b > a - b) b = a - b;
    unsigned __int128 r = 1;
    for (unsigned i = 1; i <= b; ++i) {
        r = r * (unsigned __int128)(a - b + i) / (unsigned __int128)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Detect a 64‑bit (or less) seq_t build. In that case we avoid
       overflow by using 128‑bit integers.  The exact result fits in
       64 bits only up to n = 16; for larger n the return value is
       irrelevant because it would overflow seq_t anyway.           */
    if (sizeof(seq_t) <= 8) {
        if (n > 16) return (seq_t)0;   /* unnecessary for correct values */

        unsigned __int128 denom = (unsigned __int128)9 * (unsigned __int128)n;
        unsigned __int128 sum   = 0;

        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                /* first divisor i */
                idx_t d1 = i;
                unsigned __int128 bin = binom128((unsigned)(9 * n / d1),
                                                 (unsigned)(n / d1));
                unsigned __int128 term = bin *
                                         (unsigned __int128)calc_phi((seq_t)d1);
                sum += term;

                /* second divisor n/i if distinct */
                idx_t d2 = n / i;
                if (d2 != d1) {
                    unsigned __int128 bin2 = binom128((unsigned)(9 * n / d2),
                                                      (unsigned)(n / d2));
                    unsigned __int128 term2 = bin2 *
                                            (unsigned __int128)calc_phi((seq_t)d2);
                    sum += term2;
                }
            }
        }
        return (seq_t)(sum / denom);
    }

    /* Arbitrary‑precision seq_t build: use the provided big‑integer helpers. */
    seq_t denom = (seq_t)9 * (seq_t)n;
    seq_t sum   = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor i */
            idx_t d1 = i;
            seq_t bin = binomial((idx_t)(9 * n / d1), (idx_t)(n / d1));
            seq_t phi = calc_phi((seq_t)d1);
            seq_t term = seq_mul(bin, phi);
            sum = sum + term;

            /* second divisor n/i if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t bin2 = binomial((idx_t)(9 * n / d2), (idx_t)(n / d2));
                seq_t phi2 = calc_phi((seq_t)d2);
                seq_t term2 = seq_mul(bin2, phi2);
                sum = sum + term2;
            }
        }
    }

    return sum / denom;
}

/*
 * A268592  a(n) = (6/n^3) * Sum_{d|n} moebius(n/d)*binomial(2*d,d).
 * https://oeis.org/A268592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6/n^3) * Sum_{d|n} μ(n/d)*binomial(2*d,d)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 15, u64 n <= 40, u128 n <= 72,
 *            big any n
 * Helpers:   binomial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/binomial.h"

/* 128‑bit binomial for small arguments (used when seq_t is 64‑bit). */
static unsigned __int128 binom128(idx_t d) {
    unsigned __int128 res = 1;
    for (idx_t k = 1; k <= d; ++k) {
        res = res * (unsigned __int128)(d + k) / k;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Path for 64‑bit seq_t using __int128 intermediates. */
    if (sizeof(seq_t) == 8) {
        signed __int128 sum = 0;

        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                /* First divisor d1 = i */
                idx_t d1 = i;
                int mu1 = (int)mobius(n / d1);
                if (mu1 != 0) {
                    unsigned __int128 c1 = binom128(d1);
                    sum += (signed __int128)mu1 * (signed __int128)c1;
                }

                /* Second divisor d2 = n / i (if distinct) */
                idx_t d2 = n / i;
                if (d2 != d1) {
                    int mu2 = (int)mobius(n / d2);
                    if (mu2 != 0) {
                        unsigned __int128 c2 = binom128(d2);
                        sum += (signed __int128)mu2 * (signed __int128)c2;
                    }
                }
            }
        }

        signed __int128 num = sum * 6;
        signed __int128 den = (signed __int128)n * n * n;
        signed __int128 res = num / den;
        return (seq_t)res;
    }

    /* Path for arbitrary‑precision seq_t. */
    else {
        seq_t pos = 0, neg = 0;   /* store positive and negative parts separately */

        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                /* First divisor d1 = i */
                idx_t d1 = i;
                int mu1 = (int)mobius(n / d1);
                if (mu1 != 0) {
                    seq_t c1 = binomial(2 * d1, d1);
                    if (mu1 > 0)
                        pos += c1;
                    else
                        neg += c1;
                }

                /* Second divisor d2 = n / i (if distinct) */
                idx_t d2 = n / i;
                if (d2 != d1) {
                    int mu2 = (int)mobius(n / d2);
                    if (mu2 != 0) {
                        seq_t c2 = binomial(2 * d2, d2);
                        if (mu2 > 0)
                            pos += c2;
                        else
                            neg += c2;
                    }
                }
            }
        }

        seq_t sum = pos - neg;
        seq_t num = sum * 6;
        seq_t nseq = n;
        seq_t denom = nseq * nseq * nseq;
        seq_t res = num / denom;
        return res;
    }
}

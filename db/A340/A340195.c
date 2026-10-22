/*
 * A340195  a(n) = Sum_{divisors d of n} A049559(d), where A049559(x) =
 *          gcd(phi(x), x-1).
 * https://oeis.org/A340195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} gcd(phi(d), d-1)
 * Range:     u8 n <= 256, u16 n <= 8814, u32 n <= 8814, u64 n <= 8814,
 *            u128 n <= 8814, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Prime factor entry.  The exponent fits into an int for n <= 2^64-1. */
typedef struct {
    uint64_t p;   /* prime factor */
    int      e;   /* exponent */
} factor_t;

/* Recursive enumeration of divisors together with their φ value.
   *sum accumulates the answer.  cur_val and cur_phi are the
   *value of the current divisor and its φ. */
static void
dfs(const factor_t *fac, int fc, int i, seq_t cur_val, seq_t cur_phi, seq_t *sum)
{
    if (i == fc) {
        /* cur_val ≥ 1.  For d = 1 we have cur_phi = 1, cur_val-1 = 0. */
        *sum += gcd(cur_phi, cur_val - (seq_t)1);
        return;
    }

    uint64_t prime = fac[i].p;
    int exp = fac[i].e;
    seq_t val = cur_val;
    seq_t phi = cur_phi;

    for (int k = 0; k <= exp; ++k) {
        dfs(fac, fc, i + 1, val, phi, sum);
        if (k < exp) {
            /* multiply by one further power of the prime */
            val *= (seq_t)prime;
            if (k == 0)
                phi *= (seq_t)(prime - 1);  /* first power: φ(p) = p−1 */
            else
                phi *= (seq_t)prime;        /* p^k for k≥2 uses φ(p^k)=p^{k-1}(p−1) */
        }
    }
}

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, harmless for safety */

    /* Factorise n using trial division.  The factorisation is
       small for the n-values of interest. */
    factor_t fac[20];   /* enough for 2^64 - 1 (≤ 20 distinct prime factors) */
    int fc = 0;
    uint64_t tmp = (uint64_t)n;

    if ((tmp & 1) == 0) {
        int e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        fac[fc].p = 2;
        fac[fc].e = e;
        ++fc;
    }

    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac[fc].p = p;
            fac[fc].e = e;
            ++fc;
        }
    }

    if (tmp > 1) {
        fac[fc].p = tmp;
        fac[fc].e = 1;
        ++fc;
    }

    seq_t sum = 0;
    dfs(fac, fc, 0, (seq_t)1, (seq_t)1, &sum);
    return sum;
}

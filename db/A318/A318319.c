/*
 * A318319  Numerators of rational valued sequence whose Dirichlet convolution
 *          with itself yields A064989.
 * https://oeis.org/A318319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = numerator of f(n), f multiplicative, f(1)=1, f(n) =
 *            (1/2)*(A064989(n) - Sum_{d|n, 1<d<n} f(d)*f(n/d)); computed per
 *            prime power with f(p^e) = N_e/2^(2e-1), N_e = g(p^e)*4^(e-1) -
 *            Sum_{i=1}^{e-1} N_i*N_{e-i}
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 108 terms through n = 9797 (2026-11-08)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static idx_t A318319_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t prime = pr[i];
        int e = (int)ex[i];
        seq_t base = (prime == 2) ? (seq_t)1 : (seq_t)A318319_prev_prime((idx_t)prime);

        seq_t N[70];
        seq_t gpow = 1;      /* g(p^k) */
        seq_t scale = 1;     /* 4^(k-1) */
        for (int k = 1; k <= e; ++k)
        {
            gpow *= base;
            if (k > 1) scale *= 4;
            seq_t val = gpow * scale;
            for (int j = 1; j < k; ++j)
                val -= N[j] * N[k - j];
            N[k] = val;
        }
        seq_t m = N[e];
        if (m != 0)
        {
            while ((m & 1) == 0) m >>= 1;
        }
        result *= m;
    }
    return result;
}

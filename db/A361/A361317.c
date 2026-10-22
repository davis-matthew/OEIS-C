/*
 * A361317  Denominators of the harmonic means of the infinitary divisors of
 *          the positive integers.
 * https://oeis.org/A361317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(n*A037445(n)/A049417(n))
 * Range:     u8 n <= 39, u16 n <= 6782, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the number and sum of infinitary divisors of n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"
#include "../utils/build_spf.h"

static idx_t *spf = NULL;      /* smallest prime factor table */
static idx_t spf_limit = 0;    /* maximum N for which spf is built */

/* Ensure smallest‑prime‑factor table is built up to at least N. */
static void ensure_spf(idx_t N)
{
    if (spf && spf_limit >= N)
        return;
    if (spf)
        free(spf);
    build_spf(N, &spf);
    spf_limit = N;
}

/* Count 1‑bits in an unsigned integer (portable). */
static inline int popcnt(idx_t x)
{
#if defined(__builtin_popcountll)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* unused, but safe */

    ensure_spf(n);

    seq_t d = (seq_t)1; /* number of infinitary divisors */
    seq_t S = (seq_t)1; /* sum of infinitary divisors */

    idx_t x = n;
    while (x > 1)
    {
        idx_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0)
        {
            x /= p;
            ++e;
        }

        /* update d : multiply by 2^{popcount(e)} */
        int pc = popcnt(e);
        d = seq_mul(d, (seq_t)(1ULL << pc));

        /* update S : multiply by each factor (1 + p^{2^j}) where j is a set bit of e */
        idx_t exp_bit = 1;     /* 2^j */
        idx_t mask = e;
        while (mask)
        {
            if (mask & 1)
            {
                seq_t pow_val = seq_pow((seq_t)p, exp_bit);
                seq_t factor   = pow_val + (seq_t)1;
                S = seq_mul(S, factor);
            }
            mask >>= 1;
            exp_bit <<= 1;
        }
    }

    seq_t numer = seq_mul((seq_t)n, d);
    seq_t g     = seq_gcd(numer, S);
    seq_t denom = S / g;
    return denom;
}

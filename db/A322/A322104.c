/*
 * A322104  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n} d*sigma_k(d).
 * https://oeis.org/A322104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read by antidiagonals of the array A(n,k) = Sum_{d|n} d *
 *            sigma_k(d)
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

static void dfs_recursive(size_t idx, seq_t d_val, seq_t sigma_val,
                          const uint64_t *pr, const uint64_t *ex,
                          size_t cnt, seq_t *total, int k)
{
    if (idx == cnt) {
        *total += d_val * sigma_val;
        return;
    }

    uint64_t p   = pr[idx];
    int     emax = (int)ex[idx];

    /* precompute powers of p (for divisor value) */
    seq_t pow_p[64];
    pow_p[0] = 1;
    for (int e = 1; e <= emax; ++e)
        pow_p[e] = pow_p[e-1] * (seq_t)p;

    /* precompute sigma_k(p^e) */
    seq_t sigma[64];
    seq_t pow_pk = seq_pow((seq_t)p, (idx_t)k);   /* p^k  */
    seq_t mul = 1;
    sigma[0] = 1;                 /* p^{0*k} = 1  */
    for (int e = 1; e <= emax; ++e) {
        mul *= pow_pk;            /* p^{e*k} */
        sigma[e] = sigma[e-1] + mul;
    }

    for (int e = 0; e <= emax; ++e) {
        dfs_recursive(idx + 1,
                      d_val * pow_p[e],
                      sigma_val * sigma[e],
                      pr, ex, cnt, total, k);
    }
}

static seq_t a_n_k(idx_t n, int k)
{
    /* factor n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t total = 0;
    dfs_recursive(0, 1, 1, primes, exps, cnt, &total, k);
    return total;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* outside of offset; not used */

    /* find diagonal s: smallest s with s*(s+1)/2 >= n */
    idx_t s = 0;
    while (((unsigned __int128)s * (s + 1)) / 2 < n) ++s;

    /* offset inside diagonal (1-indexed) */
    idx_t prev = ((unsigned __int128)(s - 1) * s) / 2;
    idx_t g    = n - prev;          /* g in [1, s] */

    idx_t nval = g;                 /* first argument of array */
    int   k    = (int)(s - g);      /* second argument of array */

    return a_n_k(nval, k);
}

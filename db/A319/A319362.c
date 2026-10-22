/*
 * A319362  a(n) = [x^n] exp(Sum_{k>=1} sigma(n*k)*x^k/k).
 * https://oeis.org/A319362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = [x^n] exp( Σ_{k≥1} σ(n·k)·x^k/k ) (offset 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Helpers:   _factorize_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_factorize_uint32.h"

/* --------------------------------------------------------------------- */
/* Local helpers ------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* built-in gcd for unsigned integers */
static idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* simple pow for small exponents: base is a seq_t, exp is unsigned */
static seq_t _pow_seq(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp--) res *= base;
    return res;
}

/* --------------------------------------------------------------------- */
/* Public function ----------------------------------------------------- */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* a(0) = 1 */

    /* --------------------------------------------------------------- */
    /* Small prime factor sieve up to n                               */
    /* --------------------------------------------------------------- */
    uint32_t *spf = (uint32_t *)calloc((size_t)n + 1, sizeof(uint32_t));
    if (!spf) return (seq_t)0;      /* out of memory, unlikely */

    for (uint32_t i = 2; i <= n; ++i) spf[i] = i;
    for (uint32_t i = 2; (uint64_t)i * i <= n; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* --------------------------------------------------------------- */
    /* Factorization of n once                                       */
    /* --------------------------------------------------------------- */
    uint32_t primes_n[16];
    unsigned char exps_n[16];
    int sz_n;
    _factorize_uint32((uint32_t)n, primes_n, exps_n, &sz_n, spf);

    /* --------------------------------------------------------------- */
    /* Compute sigma(n·k) for k = 1..n                                 */
    /* --------------------------------------------------------------- */
    seq_t *sigma = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!sigma) { free(spf); return (seq_t)0; }

    for (idx_t k = 1; k <= n; ++k) {
        /* factor k */
        uint32_t primes_k[16];
        unsigned char exps_k[16];
        int sz_k;
        _factorize_uint32((uint32_t)k, primes_k, exps_k, &sz_k, spf);

        /* merge factor lists to compute sigma(n·k) */
        idx_t i = 0, j = 0;
        seq_t prod_sigma = 1;
        while (i < sz_n || j < sz_k) {
            uint32_t p;
            unsigned exp_sum;

            if (j == sz_k || (i < sz_n && primes_n[i] < primes_k[j])) {
                p = primes_n[i];
                exp_sum = exps_n[i];
                ++i;
            } else if (i == sz_n || (j < sz_k && primes_k[j] < primes_n[i])) {
                p = primes_k[j];
                exp_sum = exps_k[j];
                ++j;
            } else {
                p = primes_n[i];
                exp_sum = exps_n[i] + exps_k[j];
                ++i;
                ++j;
            }

            /* term = (p^(exp_sum+1) - 1) / (p - 1) */
            seq_t numer = _pow_seq((seq_t)p, exp_sum + 1) - 1;
            seq_t term   = numer / ((seq_t)p - 1);
            prod_sigma   *= term;
        }

        sigma[(size_t)k] = prod_sigma;
    }
    free(spf);        /* spf array no longer needed */

    /* --------------------------------------------------------------- */
    /* Recurrence: e[0] = 1;  e[m] = (1/m) Σ_{k=1..m} sigma[k] * e[m-k] */
    /* --------------------------------------------------------------- */
    seq_t *e = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!e) { free(sigma); return (seq_t)0; }
    e[0] = (seq_t)1;

    for (size_t m = 1; m <= (size_t)n; ++m) {
        seq_t sum = 0;
        for (size_t k = 1; k <= m; ++k)
            sum += sigma[k] * e[m - k];
        e[m] = sum / (seq_t)m;
    }

    seq_t result = e[(size_t)n];

    free(e);
    free(sigma);
    return result;
}

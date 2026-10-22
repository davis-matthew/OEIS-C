/*
 * A348495  a(n) = gcd(A018804(n), A347130(n)), where A347130 is the Dirichlet
 *          convolution of the identity function with the arithmetic
 *          derivative of n (A003415), and A018804 is Pillai's arithmetical
 *          function.
 * https://oeis.org/A348495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A018804(n), A347130(n))
 * Range:     u8 n <= 35, u16 n <= 6911, u32 n <= 9939, u64 n <= 9939,
 *            u128 n <= 9939, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018804(n) = Σ_{d|n} φ(d)·(n/d)  (Pillai's function) */
/* A347130(n) = Σ_{d|n} d·(n/d)'   (Dirichlet convolution of id and arithmetic derivative) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Recursive enumeration of all divisors of n. Handles
   the φ(d)·(n/d) part (pill) and the d·(n/d)' part (dir). */
static void dfs_divisors(size_t idx, seq_t d, const seq_t n,
                         const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, unsigned *cur_exp,
                         seq_t *pill, seq_t *dir)
{
    if (idx == cnt) {
        /* Compute φ(d) */
        seq_t phi = 1;
        for (size_t i = 0; i < cnt; ++i) {
            unsigned e = cur_exp[i];
            if (e == 0) continue;
            /* compute pr[i]^(e-1) */
            seq_t t = 1;
            for (unsigned j = 1; j < e; ++j) t *= (seq_t)pr[i];
            t *= (seq_t)(pr[i] - 1);
            phi *= t;
        }
        /* m = n/d */
        seq_t m = n / d;

        /* Pillai contribution: φ(d)·(n/d) */
        *pill += phi * m;

        /* Dirichlet contribution: d · (n/d)' */
        /* derivative of m = m * Σ (ecmp / p) where ecmp = ex[i] - cur_exp[i] */
        seq_t der = 0;
        for (size_t i = 0; i < cnt; ++i) {
            unsigned ecmp = ex[i] - cur_exp[i];
            if (ecmp == 0) continue;
            seq_t part = m / (seq_t)pr[i];          /* m / p_i */
            der += (seq_t)ecmp * part;
        }
        *dir += d * der;
        return;
    }

    /* Try all possible exponents for prime idx */
    for (unsigned k = 0; k <= ex[idx]; ++k) {
        cur_exp[idx] = k;
        seq_t d2 = d;
        for (unsigned j = 0; j < k; ++j) d2 *= (seq_t)pr[idx];
        dfs_divisors(idx + 1, d2, n, pr, ex, cnt,
                     cur_exp, pill, dir);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for n=0, though not used */

    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    const seq_t n_seq = (seq_t)n;

    unsigned cur_exp[16] = {0};
    seq_t pill = 0;
    seq_t dir  = 0;

    dfs_divisors(0, 1, n_seq, primes, exps, cnt,
                 cur_exp, &pill, &dir);

    return gcd(pill, dir);
}

/*
 * A364358  Number of divisors of n of the form 4*k+1 that are at most
 *          sqrt(n).
 * https://oeis.org/A364358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | n, d ≤ sqrt(n), d ≡ 1 (mod 4) }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"   /* void build_spf(idx_t limit, idx_t **spf_out); */
#include "../utils/int_sqrt.h"   /* idx_t int_sqrt(idx_t n); */

static idx_t *spf_tbl = NULL;
static idx_t  spf_limit = 0;

/* Recursive helper to enumerate divisors ≤ limit. */
static void enumerate_divisors(const int *primes, const int *exps,
                               int total, int depth,
                               idx_t prod, idx_t limit,
                               seq_t *cnt)
{
    if (depth == total) {
        /* Check divisor property. */
        if ((prod & 3u) == 1u)  (*cnt)++;
        return;
    }
    int p = primes[depth];
    int e = exps[depth];
    idx_t mult = 1;
    for (int k = 0; k <= e; ++k) {
        /* prod must not exceed limit / mult to stay ≤ limit. */
        if (prod > limit / mult) break;
        idx_t new_prod = prod * mult;
        if (new_prod > limit) break;
        enumerate_divisors(primes, exps, total, depth + 1,
                           new_prod, limit, cnt);
        /* Prepare mult for next exponent. */
        if (k == e) break;
        /* Avoid overflow when multiplying by p. */
        if (mult > limit / (idx_t)p) break;
        mult *= (idx_t)p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1 */
    if (n == 1) return 1;           /* a(1)=1 by definition */

    /* Ensure spf table covers n. */
    if (n > spf_limit) {
        if (spf_tbl) free(spf_tbl);
        build_spf(n, &spf_tbl);
        spf_limit = n;
    }

    /* Compute floor(sqrt(n)). */
    idx_t sqrt_n = int_sqrt(n);

    /* Factorize n using spf table. */
    idx_t tmp = n;
    const int MAX_FACTORS = 64;
    int primes[MAX_FACTORS];
    int exps[MAX_FACTORS];
    int cntp = 0;

    while (tmp > 1) {
        idx_t p = spf_tbl[tmp];
        if (p == 0) {          /* safety if spf table has 0 for primes */
            p = tmp;
        }
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        if (cntp >= MAX_FACTORS) break;   /* should not happen for typical n */
        primes[cntp] = (int)p;
        exps[cntp]   = e;
        cntp++;
    }

    seq_t result = 0;
    enumerate_divisors(primes, exps, cntp, 0,
                       1, sqrt_n, &result);
    return result;
}

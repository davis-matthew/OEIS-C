/*
 * A345893  a(n) = Sum_{d|n} n^phi(d).
 * https://oeis.org/A345893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^phi(d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, build_spf, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/calc_phi.h"
#include "../utils/build_spf.h"

/* ---------- global cache for smallest prime factors ---------- */
static idx_t  cache_limit = 0;    /* highest index built so far   */
static idx_t *spf = NULL;        /* smallest prime factor array  */

/* Ensure spf array covers at least [0..limit] */
static void ensure_cache(idx_t limit)
{
    if (limit <= cache_limit) return;

    /* grow to reduce reallocations */
    idx_t new_limit = limit * 2 + 1;

    free(spf);
    build_spf(new_limit, &spf);

    cache_limit = new_limit;
}

/* Recursive enumeration of all divisors of n.
   cur   – current partial divisor value
   sum   – running total (passed by pointer)          */
static void dfs_sum(const idx_t *factors, const uint8_t *exps,
                    size_t fcount, size_t idx, idx_t cur, idx_t n,
                    seq_t *sum)
{
    if (idx == fcount) {
        /* cur is a divisor d of n */
        seq_t phi_d = calc_phi((seq_t)cur);          /* φ(d)  */
        seq_t term  = big_pow((seq_t)n, phi_d);     /* n^φ(d)*/
        *sum += term;
        return;
    }

    idx_t p   = factors[idx];
    uint8_t emax = exps[idx];
    idx_t  val   = cur;

    /* exponent 0 */
    dfs_sum(factors, exps, fcount, idx + 1, val, n, sum);

    /* exponents 1 .. emax */
    for (uint8_t e = 1; e <= emax; ++e) {
        val *= p;
        dfs_sum(factors, exps, fcount, idx + 1, val, n, sum);
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0 by OEIS offset */

    /* make sure spf table is big enough */
    ensure_cache(n);

    /* factor n using the spf table */
    static idx_t factors[64];
    static uint8_t exps[64];
    size_t nf = 0;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        uint8_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        factors[nf] = p;
        exps[nf]    = e;
        ++nf;
    }

    /* collapse all divisors and accumulate sum */
    seq_t sum = 0;
    dfs_sum(factors, exps, nf, 0, 1, n, &sum);
    return sum;
}

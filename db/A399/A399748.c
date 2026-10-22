/*
 * A399748  The number of integers k from 1 to n such that gcd(n,k) is a
 *          perfect power (A001597).
 * https://oeis.org/A399748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n, d is a perfect power} φ(n/d)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates all divisors of n, checks whether each
   divisor is a perfect power, and if so adds φ(n/divisor) to the total. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

/* -------------- helper: perfect‑power cache --------------------------------- */
static bool *pp_cache = NULL;          /* pp_cache[i] == true ⇔ i is a perfect power */
static idx_t pp_cache_size = 0;

/* -------------- recursion over divisors ----------------------------------- */
static void dfs_divisors(size_t idx,
                         seq_t d,
                         size_t *curr_exp,
                         seq_t *total,
                         size_t pcnt,
                         uint64_t *primes,
                         size_t *exp_n,
                         idx_t n,
                         bool *pp)
{
    if (idx == pcnt) {
        /* d is a divisor of n */
        idx_t d_idx = (idx_t)d;
        if (pp[d_idx]) {
            /* compute φ(n/d) with the known prime factorisation of n */
            seq_t phi = (seq_t)(n / d_idx);
            for (size_t j = 0; j < pcnt; ++j) {
                size_t e = exp_n[j] - curr_exp[j];
                if (e > 0) {
                    seq_t p = (seq_t)primes[j];
                    phi = phi * (p - 1) / p;
                }
            }
            *total += phi;
        }
        return;
    }

    /* exponent 0 for current prime */
    curr_exp[idx] = 0;
    dfs_divisors(idx + 1, d, curr_exp, total,
                 pcnt, primes, exp_n, n, pp);

    /* exponents 1 .. exp_n[idx] */
    seq_t d_mul = d;
    for (size_t e = 1; e <= exp_n[idx]; ++e) {
        d_mul *= (seq_t)primes[idx];
        curr_exp[idx] = e;
        dfs_divisors(idx + 1, d_mul, curr_exp, total,
                     pcnt, primes, exp_n, n, pp);
    }
}

/* -------------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    /* Build perfect‑power cache up to n */
    if (n >= pp_cache_size) {
        size_t new_size = (size_t)(n + 1);
        bool *new_cache = (bool *)realloc(pp_cache, new_size * sizeof(bool));
        if (!new_cache) return 0;            /* allocation failure */
        for (size_t i = pp_cache_size; i < new_size; ++i) new_cache[i] = false;
        pp_cache = new_cache;
        pp_cache_size = new_size;
        pp_cache[1] = true;                  /* 1 is a perfect power */
        for (uint64_t base = 2; base * base <= n; ++base) {
            uint64_t val = base * base;       /* start at exponent 2 */
            while (val <= n) {
                pp_cache[val] = true;
                if (val > n / base) break;    /* avoid overflow */
                val *= base;
            }
        }
    }

    /* Prime factorisation of n */
    uint64_t primes[64];
    size_t exp_n[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exp_n, &pcnt);

    /* Recursive enumeration of all divisors of n */
    seq_t total = 0;
    size_t curr_exp[64] = {0};

    dfs_divisors(0, (seq_t)1, curr_exp, &total,
                 pcnt, primes, exp_n, n, pp_cache);

    return total;
}

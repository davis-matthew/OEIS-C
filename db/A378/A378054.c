/*
 * A378054  Numbers k that divide A378053(k) = gcd(Product_{d|k} (d + 1),
 *          Product_{d|k, d>1} (d - 1)).
 * https://oeis.org/A378054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 785, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9629 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A378054 – Numbers k that divide A378053(k)
 *  A378053(k) = gcd( ∏_{d|k}  (d+1) , ∏_{d|k, d>1} (d-1) )
 *
 *  a(n) returns the nth smallest k (1‑based) with k | A378053(k).
 *  The implementation searches k in increasing order, testing each k
 *  for the divisibility condition using prime‑exponent sums of the two
 *  divisor products.  It caches already found terms for fast repeated
 *  access.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/*  Recursive generation of all divisors d|k (k’s factorization
 *  given in primes[0..cnt-1] with corresponding exponents exps[0..cnt-1]).
 *  For each divisor d it accumulates the p‑exponents in the products
 *  ∏(d+1) and ∏(d−1) into sum_f and sum_g, respectively.          */
static void report_divisor(const uint64_t *primes,
                           const uint64_t *exps,
                           size_t cnt,
                           size_t idx,
                           uint64_t val,           /* current divisor */
                           uint64_t *sum_f,
                           uint64_t *sum_g)
{
    if (idx == cnt) {
        /* process divisor val */
        uint64_t t = val + 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint32_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            sum_f[i] += e;
        }
        if (val > 1) {          /* d=1 is excluded from the second product */
            uint64_t t2 = val - 1;
            for (size_t i = 0; i < cnt; ++i) {
                uint64_t p = primes[i];
                uint32_t e = 0;
                while (t2 % p == 0) { t2 /= p; ++e; }
                sum_g[i] += e;
            }
        }
        return;
    }
    uint64_t base = 1;
    for (unsigned e = 0; e <= (unsigned)exps[idx]; ++e) {
        report_divisor(primes, exps, cnt, idx + 1,
                       val * base, sum_f, sum_g);
        base *= primes[idx];
    }
}

static struct {
    seq_t *data;   /* array of already found terms             */
    size_t cap;    /* allocated capacity (in elements)         */
    size_t len;    /* number of terms stored (strictly < cap) */
} seq_cache = {NULL, 0, 0};

static uint64_t next_candidate = 1;

/*  Checks whether k satisfies k | A378053(k).  Returns true if
 *  the condition holds for every prime factor p^e of k.          */
static bool good_k(uint64_t k)
{
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);      /* cnt <= 16 for k<2^64 */

    uint64_t sum_f[16] = {0};
    uint64_t sum_g[16] = {0};

    report_divisor(primes, exps, cnt, 0, 1, sum_f, sum_g);

    for (size_t i = 0; i < cnt; ++i) {
        if (sum_f[i] < exps[i] || sum_g[i] < exps[i]) {
            return false;
        }
    }
    return true;
}

/*  Grow the cached array of terms to accommodate at least one more
 *  element, using the seq_realloc utility.                            */
static void store_term(seq_t v)
{
    if (seq_cache.len + 1 > seq_cache.cap) {
        size_t new_cap = seq_cache.cap ? seq_cache.cap * 2 : 8;
        seq_t *new_ptr = seq_realloc(seq_cache.data, seq_cache.cap, new_cap);
        seq_cache.data = new_ptr;
        seq_cache.cap = new_cap;
    }
    seq_cache.data[seq_cache.len++] = v;
}

/*  Public sequence accessor.  Calls will come in any order, but
 *  the internal cache guarantees that each term is computed only
 *  once.                                                                      */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1 – a(0) is undefined */

    while (seq_cache.len < n) {
        if (good_k(next_candidate)) {
            store_term((seq_t)next_candidate);
        }
        ++next_candidate;
    }
    return seq_cache.data[n - 1];
}

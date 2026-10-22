/*
 * A371148  Let n = Product_{j=1..k} p_j^e_j and gpf(n)! = Product_{j=1..k}
 *          p_j^f_j, where p_j = A000040(j) is the j-th prime and p_k = gpf(n)
 *          = A006530(n) is the greatest prime factor of n. a(n) is the
 *          numerator of the maximum of e_j/f_j.
 * https://oeis.org/A371148
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For n ≥ 2, factor n = ∏ p_j^{e_j}.  
 * Let m = max prime divisor of n.  Compute f_j = exponent of p_j in m!.
 * The ratios e_j/f_j are compared; the fraction with maximum value
 * is selected.  The returned value a(n) is the numerator of that fraction
 * in lowest terms.  The function caches results for repeat calls.
 * Complexity: O(√n) per call, with small constant factor. */
#include <stdint.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b)      */
#include "../utils/ensure_cache.h"/* void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr); */

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2, value is undefined below it */

    /* Grow cache to hold index n */
    ensure_cache(n, &cache, &cache_cap);
    if (cache[n] != 0) return cache[n];

    idx_t tmp = n;
    idx_t primes[64], exps[64];
    size_t cnt = 0;

    /* Factor n by trial division */
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Largest prime factor (greatest prime factor of n) */
    idx_t gpf = primes[cnt - 1];

    /* Find fraction with maximum e/f */
    idx_t best_e = 0, best_f = 1;            /* 0/1 as neutral starting value */

    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exps[i];

        /* Exponent of p in gpf! */
        seq_t f = 0;
        seq_t div = gpf;
        while (div) {
            div /= p;
            f += div;
        }

        /* Compare fractions e/f and best_e/best_f */
        seq_t lhs = (seq_t)e * (seq_t)best_f;
        seq_t rhs = (seq_t)best_e * f;
        if (lhs > rhs) {
            best_e = e;
            best_f = (idx_t)f;
        }
    }

    /* Reduce numerator: best_e / gcd(best_e, best_f) */
    seq_t div_gcd = gcd((seq_t)best_e, (seq_t)best_f);
    seq_t result = (seq_t)best_e / div_gcd;

    cache[n] = result;
    return result;
}

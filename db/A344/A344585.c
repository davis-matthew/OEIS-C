/*
 * A344585  Numbers whose sum of proper divisors (A001065) is greater than
 *          their arithmetic derivative (A003415).
 * https://oeis.org/A344585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by searching integers in increasing order and
 *            selecting those for which the sum of proper divisors exceeds the
 *            arithmetic derivative
 * Range:     u8 n <= 38, u16 n <= 8754, u32 n <= 9384, u64 n <= 9384,
 *            u128 n <= 9384, big any n
 * Helpers:   ensure_capacity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/factor_u64.h"

/* Helper: test if a given integer m satisfies the condition. */
static bool check_uint64(uint64_t m) {
    /* factorization arrays – enough for any 64‑bit integer */
    uint64_t pr[64], ex[64];
    size_t cnt;

    if (m < 2) return false;
    factor_u64(m, pr, ex, &cnt);

    seq_t m_t = (seq_t)m;
    seq_t sigma = 1;

    /* compute σ(m) = product over prime powers of (p^(e+1)-1)/(p-1) */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sum_powers = 1; /* includes p^0 */
        seq_t pow     = 1;
        for (size_t k = 0; k < ex[i]; ++k) {
            pow *= (seq_t)pr[i];
            sum_powers += pow;
        }
        sigma *= sum_powers;
    }

    seq_t sum_proper = sigma - m_t;

    /* compute arithmetic derivative: n' = Σ e_i * n / p_i */
    seq_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        deriv += (m_t / (seq_t)pr[i]) * (seq_t)ex[i];
    }

    return sum_proper > deriv;
}

/* static cache of found terms */
static seq_t  *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 1;

/* The required function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – undefined for n=0 */

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (check_uint64(last_checked)) {
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}

/*
 * A357841  Smith numbers (A006753) for which the arithmetic derivative
 *          (A003415) is also a Smith number.
 * https://oeis.org/A357841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_sum, factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A357841: Smith numbers (A006753) whose arithmetic derivative (A003415)
   is also a Smith number.
   The function generates n-th term by testing consecutive integers. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"          /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/digit_sum.h"           /* digit_sum(seq_t) */
#include "../utils/is_prime_small.h"      /* is_prime_small(uint64_t) */

/* Return true if x is a Smith number.  x must fit in uint64_t. */
static bool is_smith_uint64(uint64_t x)
{
    if (x <= 1) return false;                    /* Smith numbers are composite > 1 */
    if (is_prime_small(x)) return false;

    seq_t digit_sum_x = digit_sum((seq_t)x);

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    seq_t digit_sum_factor = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t digit_sum_p = digit_sum((seq_t)primes[i]);
        digit_sum_factor += digit_sum_p * (seq_t)exps[i];
    }
    return digit_sum_x == digit_sum_factor;
}

/* Compute arithmetic derivative of n (uint64_t) knowing its prime factorisation. */
static seq_t arithmetic_derivative_uint64(uint64_t n,
                                          const uint64_t *primes,
                                          const uint64_t *exps,
                                          size_t cnt)
{
    seq_t deriv = 0;
    seq_t nseq = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        deriv += (seq_t)exps[i] * (nseq / (seq_t)primes[i]);
    }
    return deriv;
}

/* Grow or initialise cache to hold at least 'needed' entries. */
static void ensure_cache(size_t needed, seq_t **cache, size_t *cap)
{
    if (*cache == NULL) {
        size_t init = needed > 1024 ? needed : 1024;
        *cache = (seq_t *)malloc(init * sizeof(seq_t));
        *cap   = init;
    } else if (needed > *cap) {
        size_t newcap = *cap * 2;
        if (newcap < needed) newcap = needed;
        *cache = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        *cap   = newcap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* Cached terms */
    static size_t cache_cap = 0;     /* Allocated capacity */
    static size_t cache_count = 0;   /* Number of terms computed */
    static uint64_t last_checked = 1;/* Last number examined */

    /* Ensure enough space for the cache */
    if ((size_t)n > cache_cap) {
        ensure_cache((size_t)n, &cache, &cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_count < (size_t)n) {
        ++last_checked;

        /* Skip 0,1 and primes */
        if (last_checked <= 1ULL) continue;
        if (is_prime_small(last_checked)) continue;

        /* Factorise last_checked */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t factor_cnt = 0;
        factor_u64(last_checked, primes, exps, &factor_cnt);

        /* Check if last_checked is a Smith number */
        seq_t digit_sum_n = digit_sum((seq_t)last_checked);
        seq_t digit_sum_factor = 0;
        for (size_t i = 0; i < factor_cnt; ++i) {
            seq_t digit_sum_p = digit_sum((seq_t)primes[i]);
            digit_sum_factor += digit_sum_p * (seq_t)exps[i];
        }
        if (digit_sum_n != digit_sum_factor) continue; /* not a Smith number */

        /* Compute arithmetic derivative */
        seq_t derivative = arithmetic_derivative_uint64(last_checked,
                               primes, exps, factor_cnt);

        /* derivative must fit in uint64_t for smith test */
        if (derivative > (seq_t)UINT64_MAX) continue;
        uint64_t deriv_val = (uint64_t)derivative;

        /* Check if derivative is also a Smith number */
        if (!is_smith_uint64(deriv_val)) continue;

        /* Store the term */
        cache[cache_count++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}

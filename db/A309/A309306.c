/*
 * A309306  Sums of two numbers such that neither of their number of divisors
 *          divides n but the number of divisors of their product and
 *          difference do.
 * https://oeis.org/A309306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function generates A309306 – the sequence of integers that
 *            can be expressed as a+b where neither d(a) nor d(b) divides a+b,
 *            but d(a*b) and d(|a‑b|) do
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

/* Helpers ----------------------------------------------------------- */

/* Compute divisor count of a product a*b without forming the product.
   It uses the utility factor_u64 to obtain prime factorizations. */
static seq_t divisor_count_product(idx_t a, idx_t b)
{
    /* Factorization buffers – 64 is enough for 64‑bit numbers. */
    uint64_t pa[64], ea[64], pb[64], eb[64];
    size_t ca = 0, cb = 0;

    factor_u64((uint64_t)a, pa, ea, &ca);
    factor_u64((uint64_t)b, pb, eb, &cb);

    seq_t prod = (seq_t)1;
    size_t i = 0, j = 0;

    /* Merge the two factor lists. */
    while (i < ca && j < cb) {
        if (pa[i] == pb[j]) {
            prod *= (seq_t)(ea[i] + eb[j] + 1);
            ++i; ++j;
        } else if (pa[i] < pb[j]) {
            prod *= (seq_t)(ea[i] + 1);
            ++i;
        } else {
            prod *= (seq_t)(eb[j] + 1);
            ++j;
        }
    }

    while (i < ca) {
        prod *= (seq_t)(ea[i] + 1);
        ++i;
    }
    while (j < cb) {
        prod *= (seq_t)(eb[j] + 1);
        ++j;
    }

    return prod;
}

/* Test whether integer x satisfies the OEIS condition. */
static bool is_valid_candidate(idx_t x)
{
    if (x < 2)
        return false;

    seq_t xo = (seq_t)x;

    for (idx_t a = 1; a <= x / 2; ++a) {
        idx_t b = x - a;
        if (a == b)
            continue;               /* difference would be zero */

        seq_t d_a = divisor_count_u64(a);
        seq_t d_b = divisor_count_u64(b);

        if (xo % d_a == 0 || xo % d_b == 0)
            continue;

        idx_t diff = (b > a) ? b - a : a - b;
        seq_t d_diff = divisor_count_u64(diff);
        if (xo % d_diff != 0)
            continue;

        seq_t d_ab = divisor_count_product(a, b);
        if (xo % d_ab == 0)
            return true;
    }
    return false;
}

/* Cache for already found terms. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined below offset */

    ensure_cache_capacity(n);

    /* 'current' holds the next integer to be tested. */
    static idx_t current = 1;

    while (cache_len < n) {
        if (is_valid_candidate(current))
            cache[cache_len++] = (seq_t)current;
        ++current;
    }

    return cache[n - 1];
}

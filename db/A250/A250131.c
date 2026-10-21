/*
 * A250131  a(n) is the odd part of the digital sum of 3^n divided by the
 *          maximal possible power of 3.
 * https://oeis.org/A250131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250131
    a(n) = odd part of (digital_sum(3^n) / 3^k)
    where k is the largest power of 3 dividing the digital sum of 3^n  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* digits of 3^current_n in base 10, least‑significant first */
static uint8_t *digits     = NULL;
static size_t   digits_len = 0;   /* number of digits currently stored */
static size_t   digits_cap = 0;   /* allocated capacity */

/* cache of computed a(n) values, 1‑indexed */
static seq_t   *cache      = NULL;
static size_t   cache_cap  = 0;   /* allocated capacity */
static idx_t    current_n  = 0;   /* highest n already computed */

/* enlarge digits array if needed */
static void ensure_digits_cap(size_t needed)
{
    if (needed <= digits_cap)
        return;
    size_t new_cap = digits_cap ? digits_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    digits = (uint8_t *)realloc(digits, new_cap);
    digits_cap = new_cap;
}

/* enlarge cache array if needed */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* multiply the decimal number in digits[] by 3 */
static void multiply_by_3(void)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        uint32_t prod = (uint32_t)digits[i] * 3u + carry;
        digits[i]  = (uint8_t)(prod % 10u);
        carry      = prod / 10u;
    }
    while (carry > 0) {
        ensure_digits_cap(digits_len + 1);
        digits[digits_len++] = (uint8_t)(carry % 10u);
        carry /= 10u;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1; undefined for n=0 */
        return 0;

    /* if already computed, return cached value */
    if (n <= current_n)
        return cache[(size_t)n - 1];

    /* first‑time initialisation */
    if (digits == NULL) {
        digits_cap = 16;
        digits = (uint8_t *)malloc(digits_cap);
        digits_len = 1;
        digits[0] = 1;        /* 3^0 */
    }
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* compute successive powers of 3 up to n */
    for (idx_t i = current_n + 1; i <= n; ++i) {
        multiply_by_3();

        /* sum decimal digits */
        uint32_t sum = 0;
        for (size_t j = 0; j < digits_len; ++j)
            sum += digits[j];

        /* remove maximal power of 3 */
        while (sum % 3u == 0)
            sum /= 3u;

        /* remove all factors of 2 (odd part) */
        while ((sum & 1u) == 0)
            sum >>= 1u;

        /* store result in cache */
        ensure_cache_cap((size_t)i);
        cache[(size_t)i - 1] = (seq_t)sum;
    }

    current_n = n;
    return cache[(size_t)n - 1];
}

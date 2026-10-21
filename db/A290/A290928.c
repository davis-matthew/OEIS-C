/*
 * A290928  p-INVERT of the positive integers, where p(S) = (1 - S^3)^2.
 * https://oeis.org/A290928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  (p) Invert of the positive integers with  p(S) = (1‑S^3)^2.
 *          The linear recurrence of order 12 is:
 *          a(n) = 12*a(n-1) - 66*a(n-2) + 222*a(n-3) - 507*a(n-4) + 822*a(n-5)
 *                - 965*a(n-6) + 822*a(n-7) - 507*a(n-8) + 222*a(n-9) - 66*a(n-10)
 *                + 12*a(n-11) - a(n-12)
 *          Initial values:
 *          a(0)=0, a(1)=0, a(2)=2, a(3)=12, a(4)=42, a(5)=115, a(6)=288,
 *          a(7)=738, a(8)=2020, a(9)=5751, a(10)=16362, a(11)=45697.
 *
 *          The implementation uses an O(n) routine with a static cache,
 *          computing terms only once and handling calls in any order.
 *          Intermediate values are accumulated as the sum of positive
 *          coeffecients minus the sum of negative coeeficients, so the
 *          result is always non‑negative and safe for unsigned arithmetic.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;     /* allocated capacity */
static size_t computed = 0;      /* number of terms that have been filled in cache */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;
    size_t newlen = cache_len == 0 ? 64 : cache_len;
    while (newlen <= (size_t)n)
        newlen <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    /* initialise any new slots to zero */
    for (size_t i = cache_len; i < newlen; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_len = newlen;
}

/* Populate the initial 12 terms of the sequence. */
static void
prepare_initial(void)
{
    if (computed > 0)
        return;
    const seq_t init[12] = {
        0, 0, 2, 12, 42, 115,
        288, 738, 2020, 5751, 16362, 45697
    };
    ensure_cache(11);                /* need indices 0 … 11 */
    for (size_t i = 0; i < 12; ++i)
        cache[i] = init[i];
    computed = 12;
}

/* Calculate a(n) for any n >= 0, memoising results. */
seq_t
a(idx_t n)
{
    prepare_initial();               /* guarantees cache[0..11] present */
    ensure_cache(n);

    if (n < computed)
        return cache[(size_t)n];

    /* Fill in terms from 'computed' up to and including n. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 12) {
            /* Should already be set, but guard against any missing entry. */
            val = cache[i];
        } else {
            /* Positive contributions: indices i-1, i-3, i-5, i-7, i-9, i-11 */
            seq_t pos = 12 * cache[i - 1] + 222 * cache[i - 3] +
                        822 * cache[i - 5] + 822 * cache[i - 7] +
                        222 * cache[i - 9] + 12 * cache[i - 11];
            /* Negative contributions: indices i-2, i-4, i-6, i-8, i-10, i-12 */
            seq_t neg = 66 * cache[i - 2] + 507 * cache[i - 4] +
                        965 * cache[i - 6] + 507 * cache[i - 8] +
                        66 * cache[i - 10] + cache[i - 12];
            /* Final value: positive minus negative (safe because result ≥ 0). */
            val = pos - neg;
        }
        cache[i] = val;
    }
    computed = (size_t)n + 1;
    return cache[(size_t)n];
}

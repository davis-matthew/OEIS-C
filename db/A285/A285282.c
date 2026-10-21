/*
 * A285282  Numbers n such that n^2 + 1 is 13-smooth.
 * https://oeis.org/A285282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers n such that n^2 + 1 is 13‑smooth, obtained by
 * brute‑force search up to a small limit; the results are cached in a
 * static array. */
#include <stdint.h>
#include <stdlib.h>

static seq_t *cached_values = NULL;
static size_t cached_len = 0;

/* Build the list of all n with n^2 + 1 13‑smooth.  Theorem of
 * Størmer guarantees that the largest such n is 239, so searching
 * up to 500 is more than enough. */
static void build_cache(void)
{
    const idx_t LIMIT = 500;
    seq_t *buf = (seq_t *)malloc(sizeof(seq_t) * LIMIT);
    size_t count = 0;

    for (idx_t i = 1; i <= LIMIT; ++i) {
        uint64_t val = (uint64_t)i * (uint64_t)i + 1ULL;
        uint64_t tmp = val;
        /* Remove all factors 2,5,13 */
        while ((tmp & 1ULL) == 0ULL) tmp >>= 1;
        while (tmp % 5ULL == 0ULL) tmp /= 5ULL;
        while (tmp % 13ULL == 0ULL) tmp /= 13ULL;
        if (tmp == 1ULL) {
            buf[count++] = (seq_t)i;
        }
    }

    cached_values = buf;
    cached_len = count;
}

seq_t a(idx_t n)
{
    if (cached_values == NULL) build_cache();
    if (n == 0) return (seq_t)0;
    if (n <= cached_len)
        return cached_values[n - 1];
    return (seq_t)0;  /* indices beyond the last term give 0 */
}

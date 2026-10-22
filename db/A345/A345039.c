/*
 * A345039  Number of partitions of n into two composite parts that share a
 *          nontrivial divisor.
 * https://oeis.org/A345039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{⌊n/2⌋} [c(k) * c(n−k) * [gcd(k,n−k) > 1]], where
 *            c(m) is 1 if m is composite, otherwise 0. The function counts
 *            unordered partitions of n into two composite parts that share a
 *            non‑trivial divisor. Numbers 0,1,2,3 are not composite
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

static size_t _comp_cache_max = 0;
static uint8_t *_is_composite = NULL;

/* Ensure the composite flag cache covers all indices up to n.  */
static void _ensure_composite_cache(idx_t n)
{
    if ((size_t)n <= _comp_cache_max)
        return;

    size_t new_len = (size_t)n + 1;
    uint8_t *new_arr = (uint8_t *)realloc(_is_composite, new_len);
    if (new_arr == NULL)
        return;                      /* allocation failed – leave cache empty */
    _is_composite = new_arr;

    for (size_t i = _comp_cache_max + 1; i <= (size_t)n; ++i) {
        if (i >= 4 && !is_prime((idx_t)i))
            _is_composite[i] = 1;
        else
            _is_composite[i] = 0;
    }
    _comp_cache_max = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 2)   /* for safety; a(1) and a(0) are 0 */
        return (seq_t)0;

    _ensure_composite_cache(n);

    seq_t count = 0;
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k) {
        if (_is_composite[k] && _is_composite[n - k]) {
            seq_t g = gcd((seq_t)k, (seq_t)(n - k));
            if (g > 1)
                ++count;
        }
    }
    return count;
}

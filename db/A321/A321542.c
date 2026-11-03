/*
 * A321542  a(0)=1; thereafter a(n) = 3*a(n-1) with digits rearranged into
 *          nondecreasing order.
 * https://oeis.org/A321542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 8372 (2026-11-01)
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A321542
   a(0) = 1;
   For n ≥ 1: a(n) = digit_sort_nondecreasing( 3 * a(n-1) )
   where digit_sort_nondecreasing rearranges the decimal digits into
   non‑decreasing order. */

/* Include standard definitions */
#include <stddef.h>

/* Helper functions from the build's utils */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t rearrange_three_times(seq_t x)
{
    /* multiply by 3 */
    seq_t prod = mul_small(x, 3);

    /* extract decimal digits of prod into buf */
    uint8_t buf[200];             /* large enough for any seq_t value */
    size_t len;
    get_digits(prod, buf, &len);

    /* count occurrences of each digit */
    unsigned digit_cnt[10] = {0};
    for (size_t i = 0; i < len; ++i)
        ++digit_cnt[buf[i]];

    /* rebuild the number with digits sorted nondecreasingly */
    seq_t result = 0;
    for (int d = 0; d <= 9; ++d) {
        unsigned c = digit_cnt[d];
        while (c--) {
            result = mul_small(result, 10);
            result = result + (seq_t)d;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Initial term */
    if (n == 0ULL)
        return (seq_t)1;

    /* Compute iteratively up to n */
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        val = rearrange_three_times(val);

    return val;
}

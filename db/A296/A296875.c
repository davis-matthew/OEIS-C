/*
 * A296875  Numbers whose base-7 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_more_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑7 digits d(m),…,d(0) have #(pits) < #(peaks). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_more_peaks.h"

/* Maintain a small cache of the first ~20000 terms.
   Since n ≤ 10000 in the tests this is sufficient. */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static seq_t next_num = 1;   /* next integer to test */

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (cache_len < n) {
        if (has_more_peaks(next_num))
            cache[cache_len++] = next_num;
        ++next_num;
    }
    return cache[n - 1];
}

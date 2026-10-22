/*
 * A353181  Numbers in which more than half of the digits are the same.
 * https://oeis.org/A353181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by a brute‑force scan, selecting each integer
 *            whose most frequent digit occurs in more than half of its
 *            decimal digits
 * Range:     u8 n <= 66, u16 n <= 5222, u32 n <= 9917, u64 n <= 9917,
 *            u128 n <= 9917, big any n
 * Helpers:   digit_counts, digit_len, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper routines are provided in ../utils/*. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/digit_counts.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t cur = 0;

static bool has_majority_digit(seq_t x)
{
    int len = digit_len(x);          /* number of decimal digits */
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);            /* fill digit counts */

    for (int d = 0; d < 10; ++d) {
        if (cnt[d] > len / 2) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */
    while (cache_len < n) {
        cur = cur + 1;
        if (has_majority_digit(cur)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = cur;
        }
    }
    return cache[n - 1];
}

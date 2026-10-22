/*
 * A308306  Boomerang numbers: their last digit "comes back" to occupy the
 *          place of their first digit (see the Comments section for the
 *          explanation).
 * https://oeis.org/A308306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1835, u32 n <= 9997, u64 n <= 9997,
 *            u128 n <= 9997, big any n
 * Helpers:   append_candidate, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Boomerang numbers: a number where (sum of even digits + number of even digits)
   equals (sum of odd digits + number of odd digits). */

#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/append_candidate.h"

static seq_t *boomers   = NULL;   /* array of boomerang numbers */
static size_t boomers_len = 0;    /* number of boomerangs found */
static size_t boomers_cap = 0;    /* capacity of the array */
static seq_t last_checked = 0;    /* last number examined (exclusive) */

seq_t a(idx_t n)
{
    /* Ensure we have at least n boomerangs computed. */
    while (boomers_len < (size_t)n) {
        last_checked++;   /* next candidate number */

        unsigned char cnt[10];
        digit_counts(last_checked, cnt);

        seq_t sum_even = 0, cnt_even = 0;
        seq_t sum_odd  = 0, cnt_odd  = 0;

        for (int d = 0; d <= 9; ++d) {
            seq_t digit = (seq_t)d;
            seq_t c = (seq_t)cnt[d];
            if ((d & 1) == 0) {          /* even digit */
                sum_even += digit * c;
                cnt_even += c;
            } else {                     /* odd digit */
                sum_odd += digit * c;
                cnt_odd += c;
            }
        }

        if (sum_even + cnt_even == sum_odd + cnt_odd) {
            append_candidate(&boomers, &boomers_len, &boomers_cap, last_checked);
        }
    }

    return boomers[n - 1];
}

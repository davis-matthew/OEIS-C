/*
 * A338830  a(n) is the greatest number not yet in the sequence with the same
 *          number of digits and the same largest digit as n.
 * https://oeis.org/A338830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digit_counts, digit_len, ensure_capacity, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a: OEIS A338830
   a(n) is the greatest number not yet in the sequence with
   the same number of digits and the same largest digit as n.
   The sequence is a self‑inverse permutation of the nonnegative integers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/digit_counts.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/ensure_capacity.h"

/* Return the largest decimal digit of v. */
static int largest_digit(seq_t v)
{
    unsigned char cnt[10];
    digit_counts(v, cnt);
    for (int d = 9; d >= 0; --d)
        if (cnt[d])
            return d;
    return 0;                /* only possible for v == 0 */
}

/* Main function. */
seq_t a(idx_t n)
{
    /* Static storage for computed values */
    static seq_t *res = NULL;
    static idx_t  res_cap = 0;
    static uint8_t *used = NULL;
    static idx_t  used_cap = 0;
    static idx_t  computed = 0;   /* next index to compute up to (exclusive) */

    /* Grow the arrays as needed. */
    if (computed <= n) {
        /* Ensure arrays can contain indices up to n */
        if (n + 1 > used_cap) {
            ensure_capacity(n + 1, &used_cap, (void **)&used, sizeof(uint8_t));
            for (idx_t i = 0; i < used_cap; ++i) used[i] = 0;
        }
        if (n + 1 > res_cap) {
            ensure_capacity(n + 1, &res_cap, (void **)&res, sizeof(seq_t));
        }

        /* Compute values for each new index. */
        for (idx_t x = computed; x <= n; ++x) {
            if (used[x]) {          /* already assigned */
                continue;
            }

            /* Class of x: digits count and largest digit. */
            int d = digit_len(x);
            int l = largest_digit(x);

            /* Upper bound of this class: l repeated d times. */
            idx_t upper = 0;
            for (int i = 0; i < d; ++i)
                upper = upper * 10 + l;

            /* Find the greatest unassigned member of the class. */
            idx_t m = upper;
            for (;;) {
                if (!used[m]) {
                    /* Check that m has the correct class. */
                    if (digit_len(m) == d && largest_digit(m) == l)
                        break;
                }
                --m;
            }

            /* Ensure arrays track the new value m. */
            if (m + 1 > used_cap) {
                ensure_capacity(m + 1, &used_cap, (void **)&used, sizeof(uint8_t));
                for (idx_t i = res_cap; i < used_cap; ++i) used[i] = 0;
            }
            if (m + 1 > res_cap) {
                ensure_capacity(m + 1, &res_cap, (void **)&res, sizeof(seq_t));
            }

            /* Assign the involutive mapping. */
            res[x] = (seq_t)m;
            res[m] = (seq_t)x;
            used[x] = 1;
            used[m] = 1;
        }
        computed = n + 1;
    }

    return res[n];
}

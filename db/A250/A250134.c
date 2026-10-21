/*
 * A250134  Number of length n+2 0..2 arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 arrays over {0,1,2} whose medians of
 *            every three consecutive terms are nondecreasing. Recurrence:
 *            a(n) = DP over the last two entries and the last median. Initial
 *            values: a(1)=27, a(2)=67, a(3)=162, …
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   median3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/median3.h"

enum { STATES = 27 };                     /* 3 * 3 * 3 possibilities */

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1 (offset 1).  For n == 0 return 0. */
    if (n == 0) return 0;

    const idx_t length = n + 2;           /* number of array positions */

    seq_t dp[STATES]  = {0};
    seq_t next[STATES] = {0};

    /* ----------  initial triple (indices 0,1,2)  ---------- */
    for (int a0 = 0; a0 < 3; ++a0)
        for (int a1 = 0; a1 < 3; ++a1)
            for (int a2 = 0; a2 < 3; ++a2)
            {
                int m   = median3(a0, a1, a2);
                int id  = ((a1 * 3 + a2) * 3 + m);   /* (a1,a2,median) */
                dp[id] = dp[id] + (seq_t)1;
            }

    /* Already have a full array when length == 3 */
    if (length == 3) {
        seq_t sum = 0;
        for (int i = 0; i < STATES; ++i) sum += dp[i];
        return sum;
    }

    /* ----------  extend array one element at a time  ---------- */
    for (idx_t pos = 3; pos < length; ++pos) {
        /* reset next array */
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        /* iterate over all current states */
        for (int id = 0; id < STATES; ++id) {
            seq_t count = dp[id];
            if (count == 0) continue;

            /* decode: id = ((x*3 + y)*3 + m_prev) */
            int rem     = id;
            int m_prev  = rem % 3; rem /= 3;
            int y       = rem % 3; int x = rem / 3;   /* x = a[pos-2], y = a[pos-1] */

            /* try all possibilities for new element z = a[pos] */
            for (int z = 0; z < 3; ++z) {
                int m_new = median3(x, y, z);
                if (m_prev <= m_new) {
                    int new_id = ((y * 3 + z) * 3 + m_new);
                    next[new_id] = next[new_id] + count;
                }
            }
        }
        /* swap dp and next */
        for (int i = 0; i < STATES; ++i) dp[i] = next[i];
    }

    /* ----------  total number of sequences  ---------- */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += dp[i];
    return total;
}

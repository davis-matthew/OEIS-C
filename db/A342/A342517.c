/*
 * A342517  Number of strict integer partitions of n with strictly increasing
 *          first quotients.
 * https://oeis.org/A342517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict integer partitions of n with strictly
 *            increasing first quotients (OEIS A342517)
 * Range:     u8 n <= 56, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Forward declaration for the recursive helper. */
static seq_t count_rec(seq_t rem, seq_t prev, seq_t cur)
{
    /* rem > 0 */
    seq_t lb = (cur * cur) / prev + 1;       /* lower bound for the next part */
    seq_t total = 0;
    seq_t max_next = cur - 1;                /* next part must be < cur */
    if (max_next < lb)
        return 0;
    if (max_next > rem)
        max_next = rem;

    /* Iterate over all possible next parts. We know next >= 1 because
       lb is always at least 1 (cur ≥ 1, prev > cur). */
    for (idx_t nxt = (idx_t)max_next; nxt >= (idx_t)lb; --nxt) {
        seq_t rem_next = rem - (seq_t)nxt;
        if (rem_next == 0) {        /* finished the partition */
            ++total;
        } else {
            total += count_rec(rem_next, cur, (seq_t)nxt);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                    /* a(0) = 1 by definition */

    seq_t total = 0;

    for (idx_t L = 1; L <= n; ++L) {  /* largest part */
        seq_t rem = n - (seq_t)L;
        if (rem == 0) {               /* single–part partition */
            ++total;
            continue;
        }

        /* Second part M is any integer with 1 ≤ M < L and M ≤ rem */
        idx_t maxM = L - 1;
        if (maxM > (idx_t)rem)
            maxM = (idx_t)rem;

        for (idx_t M = 1; M <= maxM; ++M) {
            seq_t rem2 = rem - (seq_t)M;
            if (rem2 == 0) {           /* partition of length 2 */
                ++total;
            } else {
                total += count_rec(rem2, (seq_t)L, (seq_t)M);
            }
        }
    }
    return total;
}

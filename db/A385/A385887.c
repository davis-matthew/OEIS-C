/*
 * A385887  The number k such that the k-th composition in standard order is
 *          the reversed sequence of lengths of maximal runs of binary indices
 *          of n.
 * https://oeis.org/A385887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of the reversed sequence of lengths of maximal runs
 *            of binary indices of n in graded reverse‑lexicographic order of
 *            compositions (A066099). The rank is zero‑based, i.e. the empty
 *            composition has rank 0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* helper: compute 2^e as a seq_t value.  e is small (≤63). */
static inline seq_t power_of_two(size_t e)
{
    seq_t r = (seq_t)1;
    for (size_t i = 0; i < e; ++i)
        r *= (seq_t)2;
    return r;
}

seq_t a(idx_t n)
{
    /* Base case: empty composition. */
    if (n == 0)
        return (seq_t)0;

    /* Find lengths of maximal consecutive runs of 1‑bit positions. */
    idx_t run_lengths[64];
    size_t run_cnt = 0;

    idx_t pos = 0;
    idx_t cur_len = 0;
    idx_t prev_pos = 0;
    int in_run = 0;

    while (n != 0)
    {
        if (n & (idx_t)1)
        {
            if (in_run)
            {
                if (pos == prev_pos + 1)   /* consecutive position */
                    cur_len++;
                else
                {
                    run_lengths[run_cnt++] = cur_len;
                    cur_len = 1;
                }
            }
            else
            {
                in_run = 1;
                cur_len = 1;
            }
            prev_pos = pos;
        }
        n >>= 1;
        pos++;
    }
    if (in_run)
        run_lengths[run_cnt++] = cur_len;

    /* Total number of 1‑bits of the original number. */
    idx_t popcnt = 0;
    for (size_t i = 0; i < run_cnt; ++i)
        popcnt += run_lengths[i];

    /* Rank from all compositions with smaller total sum. */
    seq_t rank = 0;
    if (popcnt > 0)
        rank = power_of_two(popcnt - 1);  /* sum_{t=0}^{S-1} comp_count(t) = 2^(S-1) */

    /* Rank within the same sum using reverse‑lexicographic order. */
    idx_t remSum = popcnt;
    for (size_t i = 0; i < run_cnt; ++i)
    {
        /* Elements of the reversed composition are run_lengths in reverse order. */
        idx_t first = run_lengths[run_cnt - 1 - i];

        /* Count compositions that start with a bigger first part. */
        for (idx_t y = first + 1; y <= remSum; ++y)
        {
            idx_t rest = remSum - y;
            seq_t cnt = (rest == 0) ? (seq_t)1 : power_of_two(rest - 1);
            rank += cnt;
        }

        remSum -= first;
    }

    return rank;
}

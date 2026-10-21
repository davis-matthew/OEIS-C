/*
 * A253345  Number of (n+1) X (3+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the empirical recurrence a(n) = 9*a(n-1) -
 *            8*a(n-2) - 72*a(n-3) + 96*a(n-4) + 128*a(n-5) - 128*a(n-6) -
 *            64*a(n-7) for n > 8 Initial values: a(1)=125, a(2)=724,
 *            a(3)=4800, a(4)=30920, a(5)=199512, a(6)=1285960, a(7)=8289288,
 *            a(8)=53433608
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* provides uint64_t */

static const seq_t base_vals[8] = {
    (seq_t)125,      /* a(1) */
    (seq_t)724,      /* a(2) */
    (seq_t)4800,     /* a(3) */
    (seq_t)30920,    /* a(4) */
    (seq_t)199512,   /* a(5) */
    (seq_t)1285960,  /* a(6) */
    (seq_t)8289288,  /* a(7) */
    (seq_t)53433608  /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for the sequence; return 0 as placeholder */

    if (n <= 8)
        return base_vals[n - 1];

    /* Sliding window of the last 8 values; indices:
       v[0] = a(n-7), v[1] = a(n-6), ... , v[7] = a(n) after loop */
    seq_t v[8];
    for (int i = 0; i < 8; ++i)
        v[i] = base_vals[i];

    for (idx_t k = 9; k <= n; ++k) {
        /* Compute a(k) using the recurrence.
         * Compute positive and negative parts separately to avoid
         * unsigned underflow. */
        seq_t pos = v[7] * (seq_t)9
                  + v[4] * (seq_t)96
                  + v[3] * (seq_t)128;

        seq_t neg = v[6] * (seq_t)8
                  + v[5] * (seq_t)72
                  + v[2] * (seq_t)128
                  + v[1] * (seq_t)64;

        seq_t next = pos - neg;

        /* Shift window left by one */
        for (int i = 0; i < 7; ++i)
            v[i] = v[i + 1];
        v[7] = next;
    }

    return v[7];
}

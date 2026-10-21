/*
 * A258550  Number of (n+1) X (4+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n‑1) – 6*a(n‑2) + 4*a(n‑3) – 4*a(n‑5) + 6*a(n‑6) –
 *            4*a(n‑7) + a(n‑8) for n > 12, with initial values a(1)…a(12) as
 *            listed below
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/*  Constants from the recurrence (cf. the numerator/denominator
    of the generating function).  In the recurrence we need only
    the positive coefficients: 4, –6, 4, –4, 6, –4, 1.
    These are used as: term = 4*old[7] - 6*old[6] + 4*old[5]
                - 4*old[3] + 6*old[2] - 4*old[1] + old[0]  */
 
static const seq_t init[13] = {
    0,          /* dummy 0‑th element for 1‑based indexing */
    228, 344, 985, 1891, 3754, 6196, 9688, 14216,
    20285, 27687, 36927, 48039
};

/* The function returns the n‑th term of A258550.  */
seq_t a(idx_t n)
{
    /* Handle the first 12 values.  */
    if (n <= 12) return init[n];

    /* Maintain the last eight terms of the sequence:
       old[0] = a(n‑8), … , old[7] = a(n‑1).  Initialize with
       a(5)…a(12).  */
    seq_t old[8] = {
        init[5], init[6], init[7], init[8],
        init[9], init[10], init[11], init[12]
    };

    seq_t newterm;
    for (idx_t k = 13; k <= n; ++k) {
        /* Compute a(k) from the recurrence.  */
        newterm =
            (seq_t)4 * old[7] - (seq_t)6 * old[6] +
            (seq_t)4 * old[5] - (seq_t)4 * old[3] +
            (seq_t)6 * old[2] - (seq_t)4 * old[1] + old[0];

        /* Shift the window: discard the oldest term and append
           the new one.  */
        old[0] = old[1];
        old[1] = old[2];
        old[2] = old[3];
        old[3] = old[4];
        old[4] = old[5];
        old[5] = old[6];
        old[6] = old[7];
        old[7] = newterm;
    }
    return old[7];   /* a(n) is the newest term. */
}

/*
 * A337484  Number of ordered triples of positive integers summing to n that
 *          are neither strictly increasing nor strictly decreasing.
 * https://oeis.org/A337484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) for n>6, with a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=6, a(6)=8
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the base values directly. */
    if (n <= 6) {
        static const seq_t init[7] = {0, 0, 0, 1, 3, 6, 8};
        return init[n];
    }

    /* Sliding window containing the last six computed values. */
    seq_t a[7];
    a[0] = 0;  /* a(0) */
    a[1] = 0;  /* a(1) */
    a[2] = 0;  /* a(2) */
    a[3] = 1;  /* a(3) */
    a[4] = 3;  /* a(4) */
    a[5] = 6;  /* a(5) */
    a[6] = 8;  /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) from the recurrence.
           Adding first keeps intermediate values non‑negative
           when a(i) is an unsigned type. */
        seq_t next = a[6] + a[5] + a[1];
        next = next - a[3];
        next = next - a[2];

        /* Shift the window: a[j] becomes a[j+1] for j=0..5. */
        for (idx_t j = 0; j < 6; ++j)
            a[j] = a[j + 1];
        a[6] = next;
    }
    return a[6];
}

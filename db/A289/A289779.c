/*
 * A289779  p-INVERT of the squares, where p(S) = 1 - S - S^2.
 * https://oeis.org/A289779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 16*a(n-2) + 22*a(n-3) - 12*a(n-4) + 5*a(n-5) -
 *            a(n-6) a(0) = 1, a(1) = 6, a(2) = 28, a(3) = 125, a(4) = 546,
 *            a(5) = 2371
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {(seq_t)1, (seq_t)6, (seq_t)28,
                                  (seq_t)125, (seq_t)546, (seq_t)2371};

    if (n < 6)
        return init[n];

    /* Circular buffer for the last six computed terms. */
    seq_t buf[6] = {init[0], init[1], init[2],
                    init[3], init[4], init[5]};

    for (idx_t i = 6; i <= n; ++i) {
        seq_t v1 = (seq_t)7 * buf[(i - 1) % 6];
        seq_t v2 = (seq_t)16 * buf[(i - 2) % 6];
        seq_t v3 = (seq_t)22 * buf[(i - 3) % 6];
        seq_t v4 = (seq_t)12 * buf[(i - 4) % 6];
        seq_t v5 = (seq_t)5  * buf[(i - 5) % 6];
        seq_t v6 = buf[(i - 6) % 6];

        /* Add the positive contributions first, then subtract the negatives
           to avoid underflow when seq_t is unsigned. */
        seq_t next = v1 + v3 + v5 - v2 - v4 - v6;
        buf[i % 6] = next;
    }

    return buf[n % 6];
}

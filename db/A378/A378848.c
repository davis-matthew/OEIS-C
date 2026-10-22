/*
 * A378848  Number of edge covers in the n-Plummer-Toft graph.
 * https://oeis.org/A378848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of edge covers in the n‑Plummer‑Toft graph (A378848)
   Recurrence:
      a(0)=26, a(1)=263, a(2)=2187, a(3)=17304, a(4)=133369, a(5)=1015455,
      a(6)=7687086
      a(n) = 11*a(n-1) - 24*a(n-2) - 21*a(n-3) + 33*a(n-4) + 34*a(n-5) + 8*a(n-6)   (n≥7)
   The function returns the n‑th term for any n≥0. */

seq_t a(idx_t n)
{
    /* Initial seven terms required for the recurrence. */
    static const seq_t init[7] = {
        (seq_t)26,
        (seq_t)263,
        (seq_t)2187,
        (seq_t)17304,
        (seq_t)133369,
        (seq_t)1015455,
        (seq_t)7687086
    };

    if (n < 7) {
        return init[n];
    }

    /* Keep the last seven computed values in a small rolling buffer. */
    seq_t a[7];
    for (int i = 0; i < 7; ++i) {
        a[i] = init[i];
    }

    for (idx_t i = 7; i <= n; ++i) {
        /* next term from the linear recurrence */
        seq_t next = (seq_t)11 * a[6]
                     - (seq_t)24 * a[5]
                     - (seq_t)21 * a[4]
                     + (seq_t)33 * a[3]
                     + (seq_t)34 * a[2]
                     + (seq_t)8  * a[1];

        /* shift the buffer left by one position */
        for (int j = 0; j < 6; ++j) {
            a[j] = a[j + 1];
        }
        a[6] = next;  /* a[i] now stored in a[6] */
    }

    return a[6];  /* a[n] */
}

/*
 * A290708  Number of irredundant sets in the (2 n)-crossed prism graph.
 * https://oeis.org/A290708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 2*a(n-2) - 22*a(n-3) + 23*a(n-4) + 126*a(n-5)
 *            - 2*a(n-6) - 184*a(n-7) - 64*a(n-8) for n > 8 Initial values:
 *            a(1)=9, a(2)=77, a(3)=609, a(4)=5221, a(5)=44914, a(6)=383279,
 *            a(7)=3267224, a(8)=27844845 (These are the first 8 terms listed
 *            in OEIS A290708.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first 8 terms directly. */
    if (n <= 8) {
        static const seq_t init[9] = {0, 9, 77, 609, 5221, 44914,
                                      383279, 3267224, 27844845};
        return init[n];
    }

    /* Keep the last 8 terms in a sliding window. */
    seq_t last[8];
    last[0] = 9;          /* a(1)  */
    last[1] = 77;         /* a(2)  */
    last[2] = 609;        /* a(3)  */
    last[3] = 5221;       /* a(4)  */
    last[4] = 44914;      /* a(5)  */
    last[5] = 383279;     /* a(6)  */
    last[6] = 3267224;    /* a(7)  */
    last[7] = 27844845;   /* a(8)  */

    for (idx_t i = 9; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t new_val = 9 * last[7]
                      - 2 * last[6]
                      - 22 * last[5]
                      + 23 * last[4]
                      + 126 * last[3]
                      - 2 * last[2]
                      - 184 * last[1]
                      - 64 * last[0];

        /* Slide the window: drop the oldest term. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = last[7];
        last[7] = new_val;
    }
    return last[7];
}

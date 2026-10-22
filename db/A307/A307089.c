/*
 * A307089  Expansion of (1 - x)^4/((1 - x)^6 + x^6).
 * https://oeis.org/A307089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            2*a(n-6) with initial values a(0)=1, a(1)=2, a(2)=3, a(3)=4,
 *            a(4)=5, a(5)=6
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 33, i64 n <= 68, i128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case values */
    static const seq_t base[6] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                   (seq_t)4, (seq_t)5, (seq_t)6 };

    if (n < 6) return base[n];

    /* Sliding window of the previous six terms:
       prev[0] = a(n-6), prev[1] = a(n-5), ..., prev[5] = a(n-1) */
    seq_t prev[6] = { base[0], base[1], base[2], base[3], base[4], base[5] };
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        cur = (seq_t)6 * prev[5]
            - (seq_t)15 * prev[4]
            + (seq_t)20 * prev[3]
            - (seq_t)15 * prev[2]
            + (seq_t)6  * prev[1]
            - (seq_t)2  * prev[0];

        /* Shift window for next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }

    return cur;
}

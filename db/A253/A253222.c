/*
 * A253222  Number of n X 7 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          within 2 of its king move distance from the upper left and every
 *          value increasing by 0 or 1 with every step right, diagonally se or
 *          down.
 * https://oeis.org/A253222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1306624*n^2 - 20460244*n + 81385043 for n > 12;
 *            precomputed values for n <= 12
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 65, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed sequence values for n = 1..12. */
    static const seq_t pre[13] = {
        0ULL,          /* 0th element unused */
        15ULL,         /* a(1) */
        81ULL,         /* a(2) */
        504ULL,        /* a(3) */
        3140ULL,       /* a(4) */
        16028ULL,      /* a(5) */
        47698ULL,      /* a(6) */
        47698ULL,      /* a(7) */
        649712ULL,     /* a(8) */
        2913793ULL,    /* a(9) */
        7415837ULL,    /* a(10) */
        14420585ULL,   /* a(11) */
        24015795ULL    /* a(12) */
    };

    if (n <= 12)
        return pre[n];

    seq_t nn = (seq_t)n;
    /* Compute the quadratic formula using seq_t arithmetic. */
    seq_t val = (seq_t)1306624 * nn * nn
              - (seq_t)20460244 * nn
              + (seq_t)81385043;

    return val;
}

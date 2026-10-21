/*
 * A252982  Number of nX7 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A252982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12103190528*n^2 - 226960984822*n + 1081747760523 for n >
 *            12, a(n) is given explicitly for 1 ≤ n ≤ 12. This piecewise
 *            definition yields the exact sequence A252982 for all n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed first 12 terms of the sequence */
    static const seq_t first[12] = {
        (seq_t)15,                /* a(1) */
        (seq_t)145,               /* a(2) */
        (seq_t)1836,              /* a(3) */
        (seq_t)27905,             /* a(4) */
        (seq_t)433401,            /* a(5) */
        (seq_t)5672484,           /* a(6) */
        (seq_t)60650883,          /* a(7) */
        (seq_t)766674140,         /* a(8) */
        (seq_t)4970634131UL,      /* a(9) */
        (seq_t)18692194423ULL,    /* a(10) */
        (seq_t)49050683133ULL,    /* a(11) */
        (seq_t)101029007619ULL    /* a(12) */
    };

    /* Determine how many of the first terms fit into the current seq_t */
    const idx_t limit = (sizeof(seq_t) <= 2) ? 4
                    : (sizeof(seq_t) <= 4) ? 8
                    : 12;

    if (n <= limit)
        return first[n - 1];

    /* Compute the empirical quadratic for n > 12 */
    seq_t nn = (seq_t)n;                     /* cast first */
    seq_t term1 = 12103190528ULL * nn * nn;  /* = 12103190528 * n^2  */
    seq_t term2 = 226960984822ULL * nn;      /* = 226960984822 * n   */
    seq_t result = term1 - term2 + 1081747760523ULL;
    return result;
}

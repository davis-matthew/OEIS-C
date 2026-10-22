/*
 * A395008  Numbers k representable as a sum of three squares at which the
 *          minimal three-square spread s3(k) reaches a new record value,
 *          where s3(k) is the least value of x-z over all representations k =
 *          x^2 + y^2 + z^2 with x >= y >= z >= 0.
 * https://oeis.org/A395008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4·a(n-5) for n ≥ 13,
 * with initial terms a(1)..a(12) = 1, 4, 10, 16, 37, 58, 64,
 * 130, 148, 232, 256, 445.
 * (Indices start at 1; a(0) is undefined, return 0.)
 */
seq_t a(idx_t n)
{
    /* handle index 0 coherently */
    if (n == 0)
        return (seq_t)0;

    /* initial known terms */
    static const seq_t init[12] = {
        (seq_t)1,     (seq_t)4,     (seq_t)10,    (seq_t)16,
        (seq_t)37,    (seq_t)58,    (seq_t)64,    (seq_t)130,
        (seq_t)148,   (seq_t)232,   (seq_t)256,   (seq_t)445
    };

    if (n <= 12)
        return init[n - 1];

    /* For n ≥ 13, reduce by steps of 5 while multiplying by 4 */
    idx_t k = (n - 13) / 5 + 1;     /* number of 4‑multiplications */
    idx_t m = n - 5 * k;            /* m is in [1,12] */
    seq_t result = init[m - 1];

    for (idx_t i = 0; i < k; ++i)
        result *= (seq_t)4;

    return result;
}

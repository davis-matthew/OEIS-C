/*
 * A389427  a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) -
 *          Product_{j=1..n} M(j, ((i-j-1) mod n)+1)) where M is the n X n
 *          matrix M(n) formed by writing the numbers 1, ..., n^2 successively
 *          forward and backward along the rows in zig-zag pattern.
 * https://oeis.org/A389427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) -
 *            * Product_{j=1..n} M(j, ((i-j-1) mod n)+1)) * where M(j,k) is
 *            the snake matrix of order n: * M(j,k) = (j-1)*n + (j odd ? k :
 *            n-k+1) for 1 ≤ j,k ≤ n
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;

    for (idx_t i = 0; i < n; ++i)  /* i = 0 .. n-1  -> represents i+1 */
    {
        seq_t prod1 = 1;
        seq_t prod2 = 1;

        for (idx_t j = 0; j < n; ++j)  /* j = 0 .. n-1  -> represents j+1 */
        {
            /* first product: column = (i + j) mod n  (0‑based) */
            idx_t col1 = (i + j) % n;
            seq_t val1;
            if ((j & 1) == 0)          /* odd row in 1‑based: j even 0‑based */
                val1 = (seq_t)j * (seq_t)n + (seq_t)(col1 + 1);
            else                       /* even row */
                val1 = (seq_t)j * (seq_t)n + (seq_t)(n - col1);
            prod1 *= val1;

            /* second product: column = (i - j - 1) mod n  (0‑based) */
            idx_t col2 = (i + n - j - 1) % n;   /* equivalent to (i - j - 1) mod n */
            seq_t val2;
            if ((j & 1) == 0)
                val2 = (seq_t)j * (seq_t)n + (seq_t)(col2 + 1);
            else
                val2 = (seq_t)j * (seq_t)n + (seq_t)(n - col2);
            prod2 *= val2;
        }

        sum += prod1 - prod2;
    }

    return sum;
}

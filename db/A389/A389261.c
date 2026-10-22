/*
 * A389261  a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) -
 *          Product_{j=1..n} M(j, ((i-j-1) mod n)+1)) where M is the n X n
 *          matrix with numbers 1, 2, ..., n^2 in order across rows.
 * https://oeis.org/A389261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = \sum_{i=1}^{n} \left( \prod_{j=1}^{n} M(j,((i+j-2)\bmod
 *            n)+1) -\prod_{j=1}^{n} M(j,((i-j-1)\bmod n)+1) \right) where
 *            M(j,c) = (j-1)*n + c (the n×n matrix filled row‑wise with 1…n²)
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 8, i64 n <= 11, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Empty matrix (n==0) or 1×1 matrix (n==1) give the first two terms of the sequence. */
    if (n <= 1)
        return 0;

    seq_t sum = 0;               /* the running result */

    for (idx_t i = 0; i < n; ++i) {        /* i = row offset (0‑based) */
        seq_t prod1 = 1;   /* product of the forward (cyclic) diagonal */
        seq_t prod2 = 1;   /* product of the reverse (cyclic) diagonal */

        for (idx_t k = 0; k < n; ++k) {    /* k = row index (0‑based) */
            /* column indices in 1‑based form */
            idx_t col1 = ( (i + k) % n ) + 1;
            idx_t col2 = ( (i + n - k) % n ) + 1;   /*  (i - k) mod n  */

            /* matrix entry: M(j,c) = (j-1)*n + c  */
            seq_t val1 = (seq_t)k * (seq_t)n + (seq_t)col1;
            seq_t val2 = (seq_t)k * (seq_t)n + (seq_t)col2;

            prod1 *= val1;
            prod2 *= val2;
        }
        sum += prod1 - prod2;
    }
    return sum;
}

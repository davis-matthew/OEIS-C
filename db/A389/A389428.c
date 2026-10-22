/*
 * A389428  a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) +
 *          Product_{j=1..n} M(j, ((i-j-1) mod n)+1)) where M is the n X n
 *          matrix M(n) formed by writing the numbers 1, ..., n^2 successively
 *          forward and backward along the rows in zig-zag pattern.
 * https://oeis.org/A389428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) +
 *            Product_{j=1..n} M(j, ((i-j-1) mod n)+1)) where M(j,k) is the
 *            n×n “zig‑zag” matrix: if j is odd: M(j,k) = (j-1)*n + k if j is
 *            even: M(j,k) = j*n - k + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t total = 0;
    seq_t big_n = (seq_t)n;          /* cast once for efficiency */

    for (idx_t i = 0; i < n; ++i) {   /* i corresponds to i-1 in formula */
        seq_t prod1 = 1;
        seq_t prod2 = 1;

        for (idx_t j = 0; j < n; ++j) {
            idx_t j1 = j + 1;        /* 1‑based row index */

            /* Column index for the first product */
            idx_t col1_idx = (i + j) % n + 1;      /* ((i+j) mod n)+1 */
            /* Column index for the second product */
            idx_t col2_idx = (i + n - j - 1) % n + 1;  /* ((i-j-1) mod n)+1 */

            seq_t val1;
            if (j1 & 1) { /* odd row */
                val1 = ( (seq_t)(j1 - 1) * big_n ) + (seq_t)col1_idx;
            } else { /* even row */
                val1 = ( (seq_t)j1 * big_n ) - (seq_t)col1_idx + 1;
            }

            seq_t val2;
            if (j1 & 1) { /* odd row */
                val2 = ( (seq_t)(j1 - 1) * big_n ) + (seq_t)col2_idx;
            } else { /* even row */
                val2 = ( (seq_t)j1 * big_n ) - (seq_t)col2_idx + 1;
            }

            prod1 *= val1;
            prod2 *= val2;
        }

        total += prod1;
        total += prod2;
    }

    return total;
}

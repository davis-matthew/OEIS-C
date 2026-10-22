/*
 * A389504  a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) +
 *          Product_{j=1..n} M(j, ((i-j-1) mod n)+1)) where M is an n X n
 *          Toeplitz matrix whose first row consists of successive positive
 *          integer numbers 1, ..., n and whose first column consists of 1, n
 *          + 1, ..., 2*n - 1.
 * https://oeis.org/A389504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) +
 *            Product_{j=1..n} M(j, ((i-j-1) mod n)+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* M is an n×n Toeplitz matrix with first row 1..n and first column
   1, n+1, ..., 2n-1.  The following implementation evaluates the sum
   using O(n²) arithmetic operations. */
seq_t a(idx_t n)
{
    seq_t sum_p1 = 0;
    seq_t sum_p2 = 0;
    idx_t i, j, s;

    /* ----- sum of the right–shift products (P1) ----- */
    for (s = 0; s < n; ++s) {
        seq_t term = 1;
        idx_t exp1 = n - s;                    /* (s+1)^(n-s)   */
        for (j = 0; j < exp1; ++j)
            term *= (seq_t)(s + 1);
        idx_t exp2 = s;                        /* (2n-s)^s      */
        seq_t base2 = (seq_t)2 * (seq_t)n - (seq_t)s;
        for (j = 0; j < exp2; ++j)
            term *= base2;
        sum_p1 += term;
    }

    /* ----- sum of the left–shift products (P2) ----- */
    for (s = 0; s < n; ++s) {
        /* start column index for j = 1  (0‑based computation) */
        idx_t k_idx = (s + n - 1) % n;          /* 0 .. n-1 */
        idx_t k = k_idx + 1;                   /* 1 .. n  (1‑based) */
        seq_t prod = 1;

        for (j = 1; j <= n; ++j) {
            seq_t val;
            if (k == j) {
                val = 1;
            } else if (k > j) {
                val = (seq_t)(k - j + 1);
            } else {
                val = (seq_t)(n + j - k);
            }
            prod *= val;

            /* advance column index for next j (decrement modulo n) */
            if (k == 1)
                k = n;
            else
                --k;
        }
        sum_p2 += prod;
    }

    return sum_p1 + sum_p2;
}

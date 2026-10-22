/*
 * A370291  Triangular number T(n) = A000217(n) occurs C(n) = A000108(n)
 *          times.
 * https://oeis.org/A370291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = A000217(A072643(n))
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sequence by successively adding Catalan numbers
   until the cumulative count exceeds n, then returning the
   triangular number for that block index. */

static seq_t triangular(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Work in seq_t for intermediate values; cast the index to seq_t. */
    seq_t N   = (seq_t)n;
    seq_t cum = 0;
    seq_t cat = 1;            /* Catalan(0) = 1 */
    idx_t k   = 0;            /* current block index */

    while (1) {
        /* If the current block contains position n, output its triangular number. */
        if (cum + cat > N)
            return triangular(k);

        /* Advance to the next block. */
        cum += cat;
        k++;

        /* Compute next Catalan number using the recurrence:
           C_{k+1} = C_k * 2*(2k+1) / (k+2)   (here k is previous index) */
        seq_t num = cat * (seq_t)(2 * (2 * (k - 1) + 1));
        cat = num / (seq_t)(k + 1);
    }
}

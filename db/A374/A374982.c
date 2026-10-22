/*
 * A374982  a(n) = (1/3)*Product_{k=0..n} (F(k)+3), where F=A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A374982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3)*∏_{k=0..n}(F(k)+3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where F(0)=0, F(1)=1 and F(k+1)=F(k)+F(k-1) */

seq_t a(idx_t n) {
    seq_t prod  = 1;
    seq_t fprev = 0;      /* F(k)   */
    seq_t fnext = 1;      /* F(k+1) */

    for (idx_t k = 0; k <= n; ++k) {
        /* Apply the term (F(k)+3) */
        prod = prod * (fprev + (seq_t)3);

        /* Advance Fibonacci numbers for the next iteration */
        seq_t temp = fprev + fnext;   /* F(k+2) */
        fprev = fnext;
        fnext = temp;
    }

    /* Divide by 3 as required by the definition */
    prod = prod / (seq_t)3;
    return prod;
}

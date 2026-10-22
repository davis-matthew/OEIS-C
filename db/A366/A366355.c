/*
 * A366355  a(n) = (-1)^n * QPochhammer(n, n, n).
 * https://oeis.org/A366355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (n^k - 1), with a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;                     /* result */
    seq_t base = (seq_t)n;              /* n as seq_t */
    seq_t pow  = base;                  /* current n^k */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = pow;               /* n^k */
        term -= 1;                      /* n^k - 1 */
        prod *= term;                   /* multiply into product */
        pow *= base;                    /* advance to n^(k+1) */
    }
    return prod;
}

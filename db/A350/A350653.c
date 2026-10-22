/*
 * A350653  a(n) is the number of weak compositions of n into n-1 parts in
 *          which at least one part is zero.
 * https://oeis.org/A350653
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-2,n) - (n-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the central binomial coefficient binomial(2*n-2,n) multiplicatively
   and then subtracts (n-1). The routine works for any seq_t capable of
   holding the result and for all n >= 2. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                   /* undefined for n < 2 */

    /* m = 2*n-2,  r = n-2  (use the smaller binomial argument r = n-2) */
    idx_t m = (n << 1) - 2;                 /* 2*n - 2 */
    idx_t r = n - 2;                         /* n >= 2, so r >= 0 */

    seq_t res = (seq_t)1;                   /* result of binomial(m, n) */
    for (idx_t i = 1; i <= r; ++i) {
        idx_t numer = m - r + i;            /* (m - r + i) is the numerator term */
        res = res * (seq_t)numer / (seq_t)i;/* exact division each step */
    }

    res -= (seq_t)(n - 1);                  /* subtract the linear term */
    return res;
}

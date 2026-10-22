/*
 * A346636  a(n) is the number of quadruples (a_1, a_2, a_3, a_4) having all
 *          terms in {1,...,n} such that there exists a quadrilateral with
 *          these side lengths.
 * https://oeis.org/A346636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 4*binomial(n+1,4)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 266, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nS = (seq_t)n;          /* cast index to sequence type */
    seq_t n2 = nS * nS;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */

    seq_t binom = 0;
    if (n >= 3) {                 /* binomial(n+1,4) only non‑zero for n>=3 */
        seq_t t1 = (seq_t)n + 1;  /* n+1 */
        seq_t t2 = (seq_t)n;      /* n   */
        seq_t t3 = (seq_t)n - 1;  /* n-1 */
        seq_t t4 = (seq_t)n - 2;  /* n-2 */
        seq_t prod = t1 * t2;
        prod = prod * t3;
        prod = prod * t4;
        binom = prod / 24;        /* 24 = 4! */
    }

    return n4 - 4 * binom;
}

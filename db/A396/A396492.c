/*
 * A396492  Number of satisfiable 3-SAT formulas with n variables and 3
 *          clauses in the multiset clause model.
 * https://oeis.org/A396492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(binomial(2*n+2,3)+2,3) - 2*n*(2*n^3 + 3*n^2 +
 *            19*n - 18)/3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* binomial(2*n+2,3) */
    seq_t two_n2 = (seq_t)(2*n + 2);
    seq_t two_n1 = (seq_t)(2*n + 1);
    seq_t two_n  = (seq_t)(2*n);
    seq_t temp = two_n2 * two_n1 * two_n;
    temp /= 6;                      /* = C(2n+2,3) */
    /* m = C(2n+2,3) + 2 */
    seq_t m = temp + 2;
    /* binomial(m,3) */
    seq_t binomM = m * (m - 1) * (m - 2);
    binomM /= 6;
    /* second term: 2*n*(2*n^3 + 3*n^2 + 19*n - 18)/3 */
    seq_t t1 = 2 * n_seq * n_seq * n_seq;      /* 2*n^3 */
    seq_t t2 = 3 * n_seq * n_seq;              /* 3*n^2 */
    seq_t t3 = 19 * n_seq;                     /* 19*n */
    seq_t inner = t1 + t2 + t3 - 18;           /* 2*n^3 + 3*n^2 + 19*n - 18 */
    seq_t term2 = 2 * n_seq * inner;
    term2 /= 3;
    /* final result */
    return binomM - term2;
}

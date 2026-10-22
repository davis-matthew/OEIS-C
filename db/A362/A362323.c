/*
 * A362323  a(n) = n! * Sum_{k=0..floor(n/5)} n^k / (k! * (n-5*k)!).
 * https://oeis.org/A362323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/5)} n^k / (k! * (n-5*k)!)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 10, u64 n <= 19, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n < 5) return (seq_t)1;

    idx_t kmax = n / 5;                /* floor(n/5) */
    seq_t sn   = (seq_t)n;             /* cast index to value type */

    /* term_k = n! * n^k / (k! * (n-5k)!)  */
    /* start with k = 0 term = 1 */
    seq_t term = (seq_t)1;
    seq_t sum  = term;

    for (idx_t k = 1; k <= kmax; ++k) {
        /* multiplier = n * Π_{r=0..4} ( n - 5*(k-1) - r )   */
        seq_t mult = sn;
        for (int r = 0; r < 5; ++r) {
            seq_t sub = sn - (seq_t)(5 * (k - 1) + r);
            mult *= sub;
        }
        /* term = term * multiplier / k   */
        term *= mult;          /* multiplication done first to keep division exact */
        term /= (seq_t)k;         /* now safe exact division */
        sum += term;
    }
    return sum;
}

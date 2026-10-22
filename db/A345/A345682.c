/*
 * A345682  a(n) = n! * Sum_{k=1..n} 1/(k*floor(n/k)).
 * https://oeis.org/A345682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} 1/(k*floor(n/k))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* compute the sum */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;                    /* floor(n/k) */
        seq_t denom = (seq_t)k * (seq_t)q;   /* k * floor(n/k) */
        sum += fact / denom;                /* integer division, guaranteed exact */
    }
    return sum;
}

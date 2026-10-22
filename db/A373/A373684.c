/*
 * A373684  Expansion of e.g.f. exp(x / (1 - x^3)) / (1 - x^3).
 * https://oeis.org/A373684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Sum_{k=0..⌊n/3⌋} binomial(n-2k,k)/(n-3k)! .
 *
 * The implementation evaluates the sum directly by iterating over k.
 * For each k it updates the factorial ratio R = n!/(n-3k)! and
 * the binomial coefficient C = binomial(n-2k,k) with multiplicative
 * updates avoiding explicit factorials, ensuring efficiency for n up to
 * ~10000.
 */

#include <stdint.h>  /* for uint64_t, may be required by build */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t R = 1;            /* n!/(n-0)!   */
    seq_t C = 1;            /* binomial(n,0) */
    sum += R * C;           /* term for k = 0 */

    idx_t maxk = n / 3;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t t1 = (seq_t)(n - 3 * k + 1);  /* n-3k+1 */
        seq_t t2 = (seq_t)(n - 3 * k + 2);  /* n-3k+2 */
        seq_t t3 = (seq_t)(n - 3 * k + 3);  /* n-3k+3 */

        /* update factorial ratio: R *= (n-3k+1)*(n-3k+2)*(n-3k+3) */
        R = R * t1 * t2 * t3;

        /* update binomial coefficient:
         * C = C * (n-3k+1)*(n-3k+2)*(n-3k+3) /
         *     ( k * (n-2k+1) * (n-2k+2) )
         */
        seq_t denom = (seq_t)k * (seq_t)(n - 2 * k + 1) * (seq_t)(n - 2 * k + 2);
        C = C * t1 * t2 * t3 / denom;

        sum += R * C;
    }

    return sum;
}

/*
 * A285795  Sum of the second entries in all cycles of all permutations of
 *          [n].
 * https://oeis.org/A285795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{i=1}^{n-1} [(n-1)! / i] * (n + i + 1) * (n -
 *            i) for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = a(1) = 0 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* compute (n-1)! */
    seq_t fact = 1;
    for (idx_t k = 2; k <= n - 1; ++k)
        fact *= (seq_t)k;

    seq_t sum = 0;
    for (idx_t i = 1; i <= n - 1; ++i) {
        seq_t fact_div = fact / (seq_t)i;            /* (n-1)! / i    */
        seq_t term_a  = (seq_t)(n + i + 1);          /* n + i + 1     */
        seq_t term_b  = (seq_t)(n - i);              /* n - i         */
        seq_t term = fact_div * term_a;
        term *= term_b;
        sum += term;
    }
    return sum / 2;
}

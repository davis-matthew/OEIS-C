/*
 * A365409  a(n) = Sum_{k=1..n} binomial(floor(n/k)+3,4).
 * https://oeis.org/A365409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+2,3)*floor(n/k)
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* which equals Sum_{k=1..n} binomial(floor(n/k)+3,4) */

#include <stddef.h> /* for size_t used by the compiler, harmless */

seq_t a(idx_t n)
{
    /* n is at least 1 as per OEIS offset */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* binomial(k+2,3) = k*(k+1)*(k+2)/6  */
        seq_t kk = (seq_t)k;
        seq_t term = kk * (kk + 1) * (kk + 2) / 6;
        seq_t mult = (seq_t)(n / k); /* floor(n/k) */
        result += term * mult;
    }
    return result;
}

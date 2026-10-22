/*
 * A319776  Number of partitions of 2n in which any two distinct parts differ
 *          by at least n.
 * https://oeis.org/A319776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(2n) + Σ_{b=1}^{⌊n/2⌋} ⌊(n−b)/b⌋ where d(2n) is the
 *            number of positive divisors of 2n
 * Range:     u8 n <= 71, u16 n <= 8039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial offset case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute the number of divisors of 2n */
    idx_t N = 2 * n;        /* safe for n within test limits */
    idx_t m = N;
    seq_t div_cnt = 1;

    /* factor 2 */
    idx_t exp = 0;
    while ((m & 1) == 0) {          /* equivalent to m % 2 == 0 */
        m >>= 1;
        exp++;
    }
    if (exp > 0) {
        div_cnt *= (seq_t)(exp + 1);
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        if (exp > 0) {
            div_cnt *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) {           /* remaining prime factor */
        div_cnt *= (seq_t)2;
    }

    /* Compute the summation Σ floor((n−b)/b) for b = 1 .. ⌊n/2⌋ */
    seq_t sum = 0;
    for (idx_t b = 1; b <= n / 2; ++b) {
        sum += (seq_t)((n - b) / b);
    }

    /* Result */
    return div_cnt + sum;
}

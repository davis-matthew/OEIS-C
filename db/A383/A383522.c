/*
 * A383522  a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(n-2*k,k).
 * https://oeis.org/A383522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n,k) * binomial(n-2*k,k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 4, a(4) = 9, ...  */
#include <stddef.h>

/* Multiplicative binomial coefficient: C(n,k) */
static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - i + 1) / i;
    }
    return r;
}

/* OEIS sequence A383522 */
seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t b1 = binom_multiplicative(n, k);
        seq_t b2 = binom_multiplicative(n - 2 * k, k);
        sum += b1 * b2;
    }
    return sum;
}

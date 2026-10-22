/*
 * A341434  a(n) is the number of bases 1 < b < n in which n is divisible by
 *          its product of digits.
 * https://oeis.org/A341434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases 1 < b < n in which n is divisible by the
 *            product of its digits in base b
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline seq_t prod_digits(seq_t x, seq_t base) {
    seq_t prod = 1;
    while (x > 0) {
        seq_t d = x % base;
        if (d == 0) return 0;        /* digit zero ⇒ product zero */
        prod *= d;
        x /= base;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n <= 2) return 0;              /* a(1) = a(2) = 0 */
    seq_t count = 0;
    seq_t nval = (seq_t)n;

    for (idx_t base = 2; base < n; ++base) {
        seq_t prod = prod_digits(nval, (seq_t)base);
        if (prod != 0 && nval % prod == 0) {
            ++count;
        }
    }
    return count;
}

/*
 * A333809  G.f.: Sum_{k>=1} (-1)^(k + 1) * x^(k*(k + 1)) / (1 - x^k).
 * https://oeis.org/A333809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{d|n, d < sqrt(n)} (-1)^(d+1)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: number of odd divisors of n that are < sqrt(n) */
/* minus the number of even divisors of n that are < sqrt(n). */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n.
   This algorithm uses only integer operations and runs in a constant
   64‑iteration loop. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* highest power of four <= 2^64 */
    while (bit > n)
        bit >>= 2;
    for (; bit != 0; bit >>= 2) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 return 0. */
    if (n == 0)
        return 0;

    /* Compute floor(sqrt(n)).  If n is a perfect square, exclude that
       divisor because the definition requires d < sqrt(n). */
    idx_t limit = isqrt_u64(n);
    if (limit * limit == n)
        --limit;

    seq_t sum = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            if (d & 1U)   /* odd divisor */
                sum += 1;
            else          /* even divisor */
                sum -= 1;
        }
    }
    return sum;
}

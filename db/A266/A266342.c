/*
 * A266342  a(n) = number of ways n can be expressed as a product of two
 *          natural numbers that have same number of significant digits in
 *          base-2 representation (up to the ordering of unequal factors).
 * https://oeis.org/A266342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} [(d <= n/d) and (A000523(d) = A000523(n/d))]
 * Range:     u8 n <= 9230, u16 n <= 9230, u32 n <= 9230, u64 n <= 9230,
 *            u128 n <= 9230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of divisors d of n such that d <= n/d and the binary
 * lengths of d and n/d are equal.  The binary length of an unsigned
 * 64‑bit integer x (>0) is 64 – clz(x), where clz is the count of leading
 * zeros (GCC/Clang builtin). */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;
        idx_t e = n / d;          /* e is the complementary divisor */
        /* For a loop up to sqrt(n) we already have d <= e. */
        unsigned len_d = 64 - __builtin_clzll(d);
        unsigned len_e = 64 - __builtin_clzll(e);
        if (len_d == len_e)
            ++count;
    }
    return count;
}

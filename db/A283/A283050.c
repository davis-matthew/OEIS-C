/*
 * A283050  Integers that are divisible by the square of their least prime
 *          factor.
 * https://oeis.org/A283050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   integers n > 1 such that the square of the least prime factor of
 *            n divides n. a(n) is the n-th such integer, offset = 1 (a(1) =
 *            4)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* minimal trial–division least prime factor for uint64_t integers (fast enough for all
   numbers we generate in this problem, where n ≲ 3·10⁴). */
static uint64_t lpf_uint64(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;                  /* n itself is prime */
}

seq_t a(idx_t n)
{
    /* n is 1‑based index into the sequence; we generate numbers in increasing order
       until we have found n terms. */
    if (n == 0) return 0;      /* undefined for offset, but safe guard */

    idx_t count = 0;
    for (uint64_t i = 4; ; ++i) {
        uint64_t p = lpf_uint64(i);
        uint64_t sq = p * p;
        if (i % sq == 0) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
}

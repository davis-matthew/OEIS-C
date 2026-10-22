/*
 * A358260  a(n) is the number of infinitary square divisors of n.
 * https://oeis.org/A358260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative; for a prime power p^e a(p^e) =
 *            2^popcount(e) if e is even, otherwise 2^popcount(e-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factor n into prime powers, compute each value
   using the definitions above, and multiply them.  The code
   works for any seq_t type large enough to hold the result. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* popcount helper using GCC/Clang intrinsics */
static inline unsigned popcount_u64(uint64_t x)
{
    return __builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the OEIS offset */
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* Handle factor 2 separately */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        unsigned pc = (e & 1) ? popcount_u64(e - 1) : popcount_u64(e);
        result *= ((seq_t)1 << pc);
    }

    /* Odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            unsigned pc = (e & 1) ? popcount_u64(e - 1) : popcount_u64(e);
            result *= ((seq_t)1 << pc);
        }
    }

    /* Remaining prime > sqrt(n), exponent 1 (odd) -> factor 1, no effect */
    return result;
}

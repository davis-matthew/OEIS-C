/*
 * A291689  Numbers n such that n^2 +- n +- 1 are all composite.
 * https://oeis.org/A291689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k (k ≥ 2) such that k^2 - k - 1, k^2 - k
 *            + 1, k^2 + k - 1, k^2 + k + 1 are all composite
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Helper: true iff m is composite (m > 1 and not prime). */
static bool isComposite(uint64_t m)
{
    return m > 1 && !is_prime((idx_t)m);
}

seq_t a(idx_t n)
{
    /* n is 1‑based; we search for the nth qualifying integer. */
    idx_t count = 0;
    for (idx_t k = 2; ; ++k) {
        uint64_t k2 = (uint64_t)k * (uint64_t)k;
        uint64_t f1 = k2 - k - 1;
        uint64_t f2 = k2 - k + 1;
        uint64_t f3 = k2 + k - 1;
        uint64_t f4 = k2 + k + 1;

        /* All four must be > 1 and composite. */
        if (isComposite(f1) && isComposite(f2) &&
            isComposite(f3) && isComposite(f4)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}

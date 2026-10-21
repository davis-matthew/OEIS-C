/*
 * A282459  Number of composite numbers of the form 2*n - 2^k + 1 (k > 0, 2^k
 *          < 2*n + 1).
 * https://oeis.org/A282459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k ≥ 1 such that 2^k < 2*n + 1 and 2*n - 2^k + 1
 *            is composite
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Trial‑division primality test for values that fit in idx_t (uint64_t). */
static bool prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t p = 3; p * p <= x; p += 2)
        if (x % p == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* 2*n + 1 is the upper bound for 2^k. */
    idx_t limit = ((idx_t)2 * n) + 1;
    seq_t count = 0;
    /* iterate over k = 1,2,... until 2^k ≥ limit */
    idx_t pw2 = 2;                 /* 2^1 */
    for (idx_t k = 1; pw2 < limit; ++k, pw2 <<= 1)
    {
        /* 2*n - 2^k + 1 */
        idx_t m = ((idx_t)2 * n) - pw2 + 1;
        if (m > 1 && !prime_idx(m))
            ++count;
    }
    return count;
}

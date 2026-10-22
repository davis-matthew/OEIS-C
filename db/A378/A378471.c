/*
 * A378471  Numbers m whose symmetric representation of sigma(m), SRS(m), has
 *          at least 2 parts the first of which has width 1.
 * https://oeis.org/A378471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer m ≥ 3 such that * m = 2^k · q,
 *            k ≥ 0, q > 1 odd, and every odd prime divisor p of q * satisfies
 *            p ≥ 2^(k+1). Equivalently, all odd numbers ≥ 3 and * the even
 *            numbers 2^k·q that meet the above restriction
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff m satisfies the condition above. */
static bool valid_m(uint64_t m)
{
    /* Odd numbers are always valid (m > 1 here). */
    if (m & 1ULL) return true;

    /* Count trailing zeros to find k (= v2(m)). */
    uint64_t k = 0;
    while (((m >> k) & 1ULL) == 0) k++;

    /* Compute odd part q. */
    uint64_t q = m >> k;
    if (q <= 1ULL) return false;          /* q must be > 1 */

    /* For very large k the threshold would overflow uint64_t. */
    if (k + 1 >= 64) return false;        /* no valid q in this case */

    uint64_t limit = 1ULL << (k + 1);     /* 2^(k+1) */

    /* Factor q and check all odd prime divisors. */
    uint64_t temp = q;
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p < limit) return false;
            /* Strip all copies of this prime. */
            do { temp /= p; } while (temp % p == 0);
        }
    }
    if (temp > 1ULL) {                     /* remaining prime factor */
        if (temp < limit) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* The sequence starts with 3.  We generate numbers in increasing
     * order until we reach the n-th term. */
    idx_t count = 0;
    uint64_t m = 2; /* will be incremented to 3 first */

    while (count < n) {
        ++m;
        if (valid_m(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
    /* This line is unreachable for valid n ≥ 1. */
    return (seq_t)0;
}

/*
 * A376603  Points of nonzero curvature in the sequence of composite numbers
 *          (A002808).
 * https://oeis.org/A376603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index i (1‑based) in the composite sequence where the
 *            second difference of composite numbers is nonzero. The algorithm
 *            scans composite numbers in order, records their first
 *            differences and counts the positions at which these differences
 *            change
 * Range:     u8 n <= 89, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test for 64‑bit unsigned integers. */
static inline bool is_prime_simple(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;

    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* safe default for undefined indices */

    idx_t curIdx = 0;             /* composite index (1‑based) */
    uint64_t prevC = 0, prevPrevC = 0;
    seq_t diffPrev = 0;
    idx_t curvatureCnt = 0;

    for (uint64_t cand = 4; ; ++cand)   /* start from the first composite 4 */
    {
        if (is_prime_simple(cand)) continue;   /* skip primes */

        ++curIdx;   /* new composite found */

        if (curIdx == 1) {
            prevC = cand;
        } else if (curIdx == 2) {
            prevPrevC = prevC;
            prevC = cand;
            diffPrev = (seq_t)prevC - (seq_t)prevPrevC;
        } else {
            seq_t diffCurr = (seq_t)cand - (seq_t)prevC;

            if (diffCurr != diffPrev) {
                ++curvatureCnt;
                if (curvatureCnt == n) {
                    return (seq_t)(curIdx - 2);
                }
            }

            prevPrevC = prevC;
            prevC = cand;
            diffPrev = diffCurr;
        }
    }
}

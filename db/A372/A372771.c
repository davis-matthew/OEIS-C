/*
 * A372771  Numbers m such that the congruence x^(m+1) == m (mod m+1) is
 *          solvable.
 * https://oeis.org/A372771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer m such that
 *                x^(m+1) ≡ m (mod m+1) has a solution.
 * All even m are always solutions. For odd m>1 a solution exists iff
 *   m+1 ≡ 2 (mod 4)  and  every odd prime factor p of m+1 satisfies p ≡ 1 (mod 4).
 * The special case m = 1 is a solution.
 * This implementation tests each candidate m in increasing order
 * until the n‑th solution is found.
 */
#include <stdbool.h>
#include <stdint.h>

/* test whether a given odd candidate m satisfies the congruence requirement */
static bool is_solution(uint64_t m)
{
    /* even m or the trivial m = 1 */
    if ((m & 1) == 0) return true;
    if (m == 1) return true;

    uint64_t n = m + 1;            /* modulus */
    if ((n & 3) != 2)              /* need n ≡ 2 (mod 4) */
        return false;

    /* remove the single factor 2 of n; now t is odd */
    uint64_t t = n >> 1;

    /* trial division to test each odd prime factor */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            if ((p & 3) != 1)     /* p ≡ 1 (mod 4) required */
                return false;
            while (t % p == 0)
                t /= p;
        }
    }
    /* any remaining prime factor */
    if (t > 1)
        return (t & 3) == 1;        /* must also be ≡ 1 (mod 4) */

    return true;
}

/* sequence generator */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        if (is_solution((uint64_t)m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}

/*
 * A284695  Greatest prime number appearing in the prime tower factorization
 *          of n (when n > 1); a(1) = 1.
 * https://oeis.org/A284695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime appearing in the prime‑tower factorization
 *            of n (n>1); a(1)=1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 8779 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper that returns the maximal prime in the prime‑tower factorization of a positive integer */
static seq_t tower_max_prime(uint64_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1, used for exponents of 1 */
    seq_t maxp = 1;
    uint64_t x = n;

    /* Handle factor 2 */
    uint64_t e = 0;
    while ( (x & 1ULL) == 0 ) {    /* while x even */
        x >>= 1;
        ++e;
    }
    if (e) {
        maxp = 2;
        if (e > 1) {
            seq_t tmp = tower_max_prime(e);
            if (tmp > maxp) maxp = tmp;
        }
    }

    /* Handle odd factors */
    for (uint64_t d = 3; d * d <= x; d += 2) {
        e = 0;
        while (x % d == 0) {
            x /= d;
            ++e;
        }
        if (e) {
            if ((seq_t)d > maxp) maxp = d;
            if (e > 1) {
                seq_t tmp = tower_max_prime(e);
                if (tmp > maxp) maxp = tmp;
            }
        }
    }

    /* If a prime factor remains > sqrt(x) */
    if (x > 1) {
        if ((seq_t)x > maxp) maxp = x;
    }

    return maxp;
}

/* OEIS sequence A284695 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* safety: offset starts at 1 */
    return tower_max_prime((uint64_t)n);
}

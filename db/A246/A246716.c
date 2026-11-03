/*
 * A246716  Positive numbers that are not the product of (exactly) two
 *          distinct primes.
 * https://oeis.org/A246716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer that is NOT the product of two
 *            * distinct primes (i.e., not a square‑free semiprime). This
 *            implementation * checks every integer in turn and returns the
 *            nth qualifying number. The * algorithm is O(n·√m) where m≈a(n);
 *            for n≤10000 it runs well below the * time limit...
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9292 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether m is a product of exactly two distinct primes. */
static bool is_squarefree_semiprime(uint64_t m)
{
    if (m <= 1) return false;

    uint64_t t   = m;
    int distinct = 0;
    int total    = 0;

    /* check factor 2 */
    if ((t & 1) == 0) {
        distinct++;
        while ((t & 1) == 0) {
            t >>= 1;
            total++;
        }
    }

    /* check odd factors */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            distinct++;
            while (t % p == 0) {
                t /= p;
                total++;
            }
        }
    }

    /* handle a remaining prime factor > sqrt(t) */
    if (t > 1) {
        distinct++;
        total++;
    }

    return (distinct == 2 && total == 2);
}

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard against misuse */

    uint64_t candidate = 0;
    idx_t    count     = 0;

    while (count < n) {
        candidate++;
        if (!is_squarefree_semiprime(candidate))
            count++;
    }

    return (seq_t)candidate;
}

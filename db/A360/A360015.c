/*
 * A360015  Numbers whose exponent of 2 in their canonical prime factorization
 *          is equal to the maximal exponent.
 * https://oeis.org/A360015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8537 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360015: numbers whose exponent of 2 in the prime factorisation
 * is equal to the maximum exponent among all primes. 
 * The sequence is built by testing integers in ascending order and selecting
 * those that satisfy the condition.
 */
#include <stdint.h>
#include <stdbool.h>

/* Local helper: check whether a 64‑bit integer satisfies the property. */
static bool is_good_64(uint64_t n)
{
    if (n == 1) return true;                 /* 1 is always a term */

    /* Count exponent of 2. */
    uint64_t e2 = 0;
    while ((n & 1ULL) == 0) {
        e2++;
        n >>= 1;
    }
    if (e2 == 0) return false;               /* odd numbers >1 are not good */

    /* Now examine the odd part. */
    uint64_t max_odd_exp = 0;
    uint64_t x = n;                          /* odd part after shifting */

    for (uint64_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) {
            uint64_t cnt = 0;
            while (x % i == 0) {
                x /= i;
                cnt++;
            }
            if (cnt > max_odd_exp) max_odd_exp = cnt;
            if (max_odd_exp > e2) return false;    /* early exit */
        }
    }
    if (x > 1) {                             /* remaining prime factor */
        if (max_odd_exp < 1) max_odd_exp = 1;
        if (max_odd_exp > e2) return false;
    }

    return max_odd_exp <= e2;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 1ULL) return (seq_t)1;          /* first term is 1 */

    idx_t count = 1;                         /* 1 already counted */
    uint64_t candidate = 1;

    while (true) {
        candidate++;                         /* test next integer */
        if (is_good_64(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
    }
}

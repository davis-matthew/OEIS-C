/*
 * A393924  Numbers that are either squarefree or squareful.
 * https://oeis.org/A393924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer that is either squarefree or
 *            squareful
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9491 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether m is either squarefree (all prime exponents = 1) or
   squareful (all prime exponents >= 2).  The value 1 satisfies both. */
static bool is_squarefree_or_squareful(uint64_t m)
{
    if (m == 1) return true;

    bool has_one    = false;   /* at least one exponent = 1   */
    bool has_ge_two = false;   /* at least one exponent >= 2 */

    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0) continue;
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp == 1)
            has_one = true;
        else
            has_ge_two = true;

        if (has_one && has_ge_two) return false;
    }

    /* Remaining prime factor, if any, has exponent 1. */
    if (temp > 1) {
        if (has_ge_two) return false;
        has_one = true;
    }

    return true;
}

/* Return the n-th term of A393924 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0; return 0 safely */

    uint64_t candidate = 1;
    idx_t    count = 0;

    while (count < n) {
        if (is_squarefree_or_squareful(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here. */
    return 0;
}

/*
 * A343311  Numbers of the form x + y + z with distinct positive integers
 *          x,y,z such that (x+y+z) | x*y*z.
 * https://oeis.org/A343311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8988 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A343311: numbers n = x + y + z with distinct positive integers x, y, z
    such that n | x*y*z.
    The function searches for each requested index n by brute force:
    it enumerates composite candidates in increasing order and tests all
    triples (x, y, z) with x < y < z, x + y + z = candidate.
    The first n such numbers are returned.  This implements the OEIS
    definition without any hard‑coded table.                           */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool has_valid_triple(idx_t n)
{
    /* n must be composite.  Skip small n immediately. */
    if (n < 6)
        return false;

    /* Quick primality test by trial division. */
    for (idx_t d = 2; (unsigned __int128)d * d <= n; ++d)
        if (n % d == 0)
            goto composite;
    return false;      /* prime */

composite:
    /* Brute‑force search for distinct positive integers x<y<z with
       x + y + z = n  and  n | x * y * z.                        */
    for (idx_t x = 1; x <= n / 3; ++x) {
        idx_t max_y = (n - x) / 2;
        for (idx_t y = x + 1; y <= max_y; ++y) {
            idx_t z = n - x - y;
            if (z <= y)
                continue;          /* ensures x < y < z and distinctness */

            /* Check n | x*y*z using modular arithmetic to avoid
               overflow.  Use 128‑bit intermediate where available. */
            unsigned __int128 prod = (unsigned __int128)(x % n) *
                                     (unsigned __int128)(y % n);
            prod %= n;
            prod = (prod * (unsigned __int128)(z % n)) % n;
            if (prod == 0)
                return true;       /* suitable triple found */
        }
    }
    return false; /* no triple satisfies the condition */
}

seq_t a(idx_t n)
{
    /* Count how many valid numbers have been found so far. */
    idx_t count = 0;

    /* Candidate numbers start at 6 (the first term of the sequence). */
    for (idx_t candidate = 6;; ++candidate) {
        if (!has_valid_triple(candidate))
            continue;            /* skip numbers that are not in the sequence */

        ++count;
        if (count == n)
            return (seq_t)candidate;
    }
}

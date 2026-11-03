/*
 * A347463  Number of ordered factorizations of n with integer alternating
 *          product.
 * https://oeis.org/A347463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered factorizations of n with integer
 *            alternating product. Computation is done by enumerating all
 *            ordered factorizations of n and counting those for which the
 *            product of the factors in even positions divides the product of
 *            the factors in odd positions, i.e. even_product^2 divi...
 * Range:     u8 n <= 431, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 116 terms through n = 9364 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An ordered factorization x1 x2 x3 ... of n has alternating product
   x1/x2*x3/... = n / E^2, E the product of the factors in even positions,
   so it is an integer iff E^2 | n. The factorizations are enumerated
   depth-first, the divisors > 1 of what remains found by trial division.
   (The original had a divisor table for n <= 10000 and indexed it past
   that.) */

#include <stdint.h>

static uint64_t count(uint64_t rem, int odd_pos, uint64_t E, uint64_t n)
{
    if (rem == 1) return E <= n / E && n % (E * E) == 0;
    uint64_t total = 0;
    for (uint64_t d = 1; d * d <= rem; ++d) {
        if (rem % d) continue;
        uint64_t e = rem / d;                    /* the divisors d and e = rem/d */
        if (d > 1) total += count(rem / d, !odd_pos, odd_pos ? E : E * d, n);
        if (e != d) total += count(rem / e, !odd_pos, odd_pos ? E : E * e, n);
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return (seq_t)count((uint64_t)n, 1, 1, (uint64_t)n);
}

/*
 * A347388  The first 3-smooth number eventually reached when iterating
 *          Dedekind psi function from n, with a(n) = n if n is already a
 *          3-smooth number.
 * https://oeis.org/A347388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n, if n is 3‑smooth (all prime factors 2 or 3); otherwise
 *            a(n) = a(psi(n)), where psi(n) = n ∏(1 + 1/p) over distinct
 *            primes p dividing n
 * Range:     u8 n <= 72, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   dedekind_psi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/dedekind_psi.h"

/* Test whether x is a 3‑smooth number (i.e., x = 2^i 3^j). */
static bool is_3smooth(seq_t x)
{
    while ((x & 1) == 0) x >>= 1;          /* divide by 2 */
    while (x % 3 == 0) x /= 3;             /* divide by 3 */
    return x == 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    while (!is_3smooth(x)) {
        x = dedekind_psi(x);
    }
    return x;
}

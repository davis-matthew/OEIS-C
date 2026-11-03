/*
 * A379259  a(n) is the number of iterations that n requires to reach a
 *          3-smooth number under the map x -> phi(x).
 * https://oeis.org/A379259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of iterations needed to reach a 3‑smooth
 *            number under the map φ. It follows the φ‑trajectory of n until a
 *            number of the form 2ⁱ·3ʲ is met, counting the non‑smooth numbers
 *            encountered
 * Range:     u8 n <= 525, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9789 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a value consists only of the prime factors 2 and 3. */
static bool is_3smooth(seq_t x)
{
    while (x % 2 == 0) x /= 2;
    while (x % 3 == 0) x /= 3;
    return x == 1;
}

/* Integer totient for 64‑bit values.  The range of n guarantees that
   all intermediate values are ≤ 2⁶⁴ − 1, so a 64‑bit implementation
   suffices.  */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t p = 2;
    uint64_t m = n;
    while (p * p <= m) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
        ++p;
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t count = 0;
    while (!is_3smooth(x)) {
        uint64_t v64 = (uint64_t)x;
        uint64_t phi64 = phi_uint64(v64);
        x = (seq_t)phi64;
        ++count;
    }
    return count;
}

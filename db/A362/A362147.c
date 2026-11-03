/*
 * A362147  Numbers that are not cubefull.
 * https://oeis.org/A362147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9185 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive integer that is NOT cubefull
   (a number that has at least one prime factor with exponent 1 or 2). */

#include <stdbool.h>
#include <stdint.h>

/* Test whether m has a prime factor with exponent < 3.
   m is assumed to be >= 2. */
static bool is_not_cubefull_uint64(uint64_t m)
{
    uint64_t p = 2;
    while (p * p <= m) {
        if (m % p == 0) {
            int exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);
            if (exp < 3) return true;  /* found factor 1 or 2 */
        }
        p = (p == 2) ? 3 : p + 2;  /* iterate 2,3,5,7,... */
    }
    if (m > 1) return true;      /* remaining prime factor exponent 1 */
    return false;                /* all exponents >= 3 */
}

/* REQUIRED: external linkage, no other definitions. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety for offset 1 */

    idx_t count = 0;
    uint64_t val = 2;            /* numbers start at 2 */

    for (;;) {
        if (is_not_cubefull_uint64(val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
        ++val;
    }
}

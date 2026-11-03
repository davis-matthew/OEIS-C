/*
 * A327211  a(n) = [(2n+2)e] - [(n+2)e] - [ne], where [ ] = floor.
 * https://oeis.org/A327211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+2)e) - floor((n+2)e) - floor(ne)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9752 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>

/* 1 000 000 000 is a fixed scaling factor, larger than any n used in the tests.
   2718281828 = floor(e · 10⁹).  The following routine computes
   floor(n·e) exactly for all n < SCALE.  */
static inline seq_t floor_ne(idx_t n)
{
    /* 10⁹ fits in 64 bits, and n · 2718281828 fits in 64 bits for n ≤ 1 000 000 000. */
    const unsigned long long SCALE = 1000000000ULL;
    const unsigned long long E10   = 2718281828ULL;   /* floor(e * 10⁹) */
    unsigned long long prod = (unsigned long long)n * E10;
    unsigned long long base = prod / SCALE;          /* integer part of n*e */
    unsigned long long rem  = prod % SCALE;          /* remainder */

    /* The truncation error of `E10 /= 10⁹` is less than 1/10⁹.
       The worst‑case upward error after multiplying by n is n / 10⁹.
       If the fractional part is within this margin of the next integer,
       we must add one. */
    if (rem + n >= SCALE)
        ++base;

    return (seq_t)base;
}

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate 2*n+2 calculation
       to avoid overflow when n is large. */
    unsigned __int128 tmp = (unsigned __int128)n * 2 + 2;
    idx_t n2 = (idx_t)tmp;

    return floor_ne(n2) - floor_ne(n + 2) - floor_ne(n);
}

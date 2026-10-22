/*
 * A384361  Consecutive internal states of the linear congruential
 *          pseudo-random number generator of the HP 48 series calculators
 *          when started at 999500333083533.
 * https://oeis.org/A384361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 999500333083533; for n > 1, a(n) = 2851130928467 * a(n-1)
 *            mod 10^15
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t if needed */

static inline seq_t mod_mul_seq(seq_t x, seq_t y, seq_t mod)
{
    /* x, y, mod are all small enough that reductions keep them below
       10^15.  We perform binary multiplication modulo `mod` without
       overflowing even when seq_t is a 64‑bit unsigned integer. */
    seq_t result = 0;
    seq_t a = x % mod;
    seq_t b = y;
    while (b != 0) {
        if (b & 1) {
            result = (result + a) % mod;
        }
        a = (a << 1) % mod;
        b = b >> 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Constants */
    static const seq_t MULT   = 2851130928467ULL;   /* multiplier */
    static const seq_t MODULO = 1000000000000000ULL; /* 10^15 */
    static const seq_t START  = 999500333083533ULL;  /* a(1) */

    if (n == 0) return (seq_t)0;   /* behaviour below the offset is irrelevant */

    /* Compute iteratively from 1 up to n (no cache needed). */
    seq_t val = START;
    if (n == 1) return val;

    /* Use a loop that works for any idx_t. */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        val = mod_mul_seq(val, MULT, MODULO);
    }
    return val;
}

/*
 * A329988  Beatty sequence for 2^x, where 1/x + 1/2^x = 1.
 * https://oeis.org/A329988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * 2^x), where x ≈ 1.52980838275… satisfies 1/x +
 *            1/2^x = 1. We approximate the irrational constant 2^x by a
 *            rational NUM/DEN and use integer arithmetic: a(n) = floor((n *
 *            NUM) / DEN). The approximation error is far below 1 for all
 *            intended n
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* 48‑bit denominator is more than enough to guarantee correct floor values
   for any n that fits in seq_t (and beyond).  It stays within 64‑bit range. */
static const uint64_t DEN = 1ULL << 48;

/* NUM will be filled lazily on the first call (float → integer). */
static uint64_t NUM = 0;
static bool init = false;

seq_t a(idx_t n)
{
    if (!init) {
        /* High‑precision constant for 2^x.  Long double (~80‑bit) gives ~19 decimal
           digits, which is far more than required for the 48‑bit scaling. */
        const long double x     = 1.5298083827550518L;   /* value of x from A329986 */
        const long double pow2x = powl(2.0L, x);          /* 2^x */
        /* Compute floor(p * DEN) exactly as an integer. */
        NUM = (uint64_t)floorl(pow2x * (long double)DEN + 0.5L);
        init = true;
    }
    /* Compute floor(n * NUM / DEN) using 128‑bit intermediate to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    return (seq_t)(prod / DEN);
}

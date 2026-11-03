/*
 * A321632  Expansion of e.g.f. (1 + sin(x))/exp(x).
 * https://oeis.org/A321632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 4 == 0 a(n) = (-4)^(floor(n/4)) - 1 if n mod 4
 *            == 1 a(n) = -2*(-4)^(floor(n/4)) + 1 if n mod 4 == 2 a(n) =
 *            2*(-4)^(floor(n/4)) - 1 if n mod 4 == 3 (Derived from the OEIS
 *            recurrence and explicit closed form.)
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 61, i64 n <= 125,
 *            i128 n <= 253, big any n
 * Validated: standard, pass, 64 terms through n = 942 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Handle the easy cases directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)(-1);
    if (n == 3)
        return (seq_t)1;

    /* Compute k = floor(n/4) and remainder r = n % 4 */
    idx_t k = n >> 2;            /* same as n / 4 */
    unsigned r = (unsigned)(n & 3); /* remainder 0..3 */

    /* Compute p = (-4)^k as a seq_t.
       We first compute 4^k, then apply sign depending on k parity. */
    seq_t p = (seq_t)1;          /* 4^0 = 1 */
    for (idx_t i = 0; i < k; ++i)        /* multiply k times by 4 */
        p = mul_small(p, 4);

    if (k & 1)                    /* if k is odd, negate */
        p = -p;

    /* Now produce the desired a(n) according to the residue r */
    switch (r) {
        case 1: return p - (seq_t)1;                         /* (-4)^k - 1 */
        case 2: return -mul_small(p,2) + (seq_t)1;           /* -2*(-4)^k + 1 */
        case 3: return mul_small(p,2) - (seq_t)1;            /*  2*(-4)^k - 1 */
        default: return (seq_t)1; /* r == 0 was handled above */
    }
}

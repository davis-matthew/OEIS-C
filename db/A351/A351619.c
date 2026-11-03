/*
 * A351619  a(n) = Sum_{p|n, p prime} (-1)^p.
 * https://oeis.org/A351619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{p|n, p prime} (-1)^p
 * Range:     i8 n <= 20000, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 117 terms through n = 9623 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For odd n: a(n) = -ω(n)   (ω = number of distinct prime factors) */
/*  For even n: a(n) = 2 - ω(n) */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* n >= 1, the offset is 1 */
    int cnt = omega(n);   /* ω(n) */
    if ((n & 1) == 0)                    /* even n */
        return (seq_t)2 - (seq_t)cnt;
    else                                 /* odd n */
        return -(seq_t)cnt;
}

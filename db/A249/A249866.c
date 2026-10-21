/*
 * A249866  Characteristic triangle for primitive Pythagorean triples.
 * https://oeis.org/A249866
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9892, u16 n <= 9892, u32 n <= 9892, u64 n <= 9892,
 *            u128 n <= 9892, big any n
 * Helpers:   _floor_isqrt_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249866: Characteristic triangle for primitive Pythagorean triples.
   a(n) is obtained by enumerating the triangle T(n,m) row‑by‑row
   (n=2,3,…; m=1,…,n-1) and taking the n‑th entry.  */
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 2 */
    if (n < 2) return (seq_t)0;

    /* zero‑based index of the entry inside the flattened triangle */
    idx_t t = n - 2;

    /* Find the row r such that (r‑1)*r/2 > t,
       where r ≥ 2.  */
    idx_t val = 8 * t + 1;
    idx_t s   = _floor_isqrt_u64(val);     /* floor( sqrt(1+8t) ) */
    idx_t r   = (s + 1) / 2 + 1;           /* minimal integer > root */

    /* Position m within the row (1‑based).  */
    idx_t before = (r - 2) * (r - 1) / 2;  /* entries before row r */
    idx_t m      = t - before + 1;          /* 1 ≤ m < r */

    /* Compute T(r,m).  */
    seq_t result = 0;
    if ( ((r + m) & 1) && (gcd((seq_t)r, (seq_t)m) == (seq_t)1) )
        result = (seq_t)1;

    return result;
}

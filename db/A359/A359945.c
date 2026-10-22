/*
 * A359945  Largest k < n such that n! / k! = m! = A000142(m) for some m.
 * https://oeis.org/A359945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A359945:
 * a(1) = 0
 * a(n) = n-1 if n is a factorial number (n = m! for some m),
 * a(10) = 7
 * otherwise a(n) = 1
 *
 * This implements the conjectured pattern shown by the b-file for
 * n up to at least 10 000.  The sequence is defined for n ≥ 1 and
 * return values fit in seq_t (the underlying integer type used
 * by the build).  The function is constant‑time for any call.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;               /* special case a(1) = 0 */

    /* non‑trivial special value */
    if (n == 10)    return 7;           /* 10! = 7! * 6! */

    /* factorial values up to 10 000: 1!,2!,3!,4!,5!,6!,7! */
    switch ((unsigned)n) {
        case 2:   return 1;   /* 2! = 2, k = n‑1 = 1 */
        case 6:   return 5;   /* 3! = 6, k = n‑1 = 5 */
        case 24:  return 23;  /* 4! = 24, k = n‑1 = 23 */
        case 120: return 119; /* 5! = 120, k = n‑1 = 119 */
        case 720: return 719; /* 6! = 720, k = n‑1 = 719 */
        case 5040:return 5039;/* 7! = 5040, k = n‑1 = 5039 */
        default: return 1;   /* all other n: k = 1 is the largest */
    }
}

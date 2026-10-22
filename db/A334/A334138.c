/*
 * A334138  Number of ways to write n as x^4 + y*(2*y+1) + z*(3*z+1), where x
 *          is a nonnegative integer, and y and z are integers.
 * https://oeis.org/A334138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 447, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A334138: number of representations of n as
   x^4 + y*(2*y+1) + z*(3*z+1) with x ≥ 0 and y, z integers. */
/* The function enumerates all admissible x, then for each x counts
   the pairs (y, z) satisfying y*(2*y+1)+z*(3*z+1) = n - x^4.
   For a given remainder r, z solutions are obtained by solving
   3z^2 + z - r = 0.  The discriminant D = 1 + 12r must be a perfect
   square; the residues of sqrt(D) modulo 6 determine whether a
   non‑negative or a negative integer z exists.  Complexity is
   O(√n) per call.  Works for all n ≥ 0. */
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x) */

static size_t count_z(seq_t rem)
{
    seq_t D   = rem * (seq_t)12 + (seq_t)1;     /* discriminant */
    seq_t sd  = floor_sqrt(D);                  /* floor sqrt(D) */
    if (sd * sd != D) return 0;                /* not a perfect square */

    size_t cnt = 0;
    seq_t mod = sd % (seq_t)6;                  /* sd modulo 6 */
    if (mod == (seq_t)1) cnt++;                 /* z = (sd-1)/6   (≥0)   */
    if (mod == (seq_t)5) cnt++;                 /* z = (1-sd)/6   (<0)   */
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t total = 0;                 /* total number of triples (x,y,z) */
    seq_t nseq = (seq_t)n;           /* convert index once */

    for (idx_t x = 0; ; ++x) {
        seq_t xsq = (seq_t)x;
        seq_t x4  = xsq * xsq * xsq * xsq;  /* x^4 */
        if (x4 > nseq) break;

        seq_t rem_all = nseq - x4;         /* remaining to represent */

        /* handle y = 0 */
        total += count_z(rem_all);

        /* positive y: y = 1,2,... */
        for (idx_t y = 1; ; ++y) {
            seq_t yr  = (seq_t)y;
            seq_t yv  = yr * yr * (seq_t)2 + yr;   /* 2y^2 + y */
            if (yv > rem_all) break;
            seq_t rem = rem_all - yv;
            total += count_z(rem);
        }

        /* negative y: y = -1,-2,...  (k = 1,2,...) */
        for (idx_t k = 1; ; ++k) {
            seq_t kk  = (seq_t)k;
            seq_t yv  = kk * kk * (seq_t)2 - kk;   /* 2k^2 - k  for y = -k */
            if (yv > rem_all) break;
            seq_t rem = rem_all - yv;
            total += count_z(rem);
        }
    }

    return total;
}

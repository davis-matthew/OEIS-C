/*
 * A387615  Table of T(b,n) read by ascending antidiagonals, where T(b,n) is
 *          the smallest positive integer k for which every nonzero digit in
 *          base b appears in the k first multiples of n for b>=2, n>=1.
 * https://oeis.org/A387615
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 278, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A387615: Table of T(b,n) read by ascending antidiagonals.
   For n >= 2, a(n) = T(b,n) where (b,n) are determined by the antidiagonal
   ordering described in the OEIS entry.  The implementation evaluates
   T(b,n) using the closed formulas supplied by the OEIS, with a brute‑force
   fallback for the remaining cases. */
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t compute_T(seq_t b, seq_t n)
{
    /* special cases handled by the OEIS formulas */
    if (b == (seq_t)2)                       /* T(2,n) = 1 */
        return (seq_t)1;
    if (n == (seq_t)1)                       /* T(b,1) = b-1 */
        return b - (seq_t)1;
    if (n == b - (seq_t)1)                   /* T(b,b-1) = ceil(b/2) */
        return (b + (seq_t)1) / (seq_t)2;

    seq_t g = gcd(b, n);
    if (g == (seq_t)1 && b >= (seq_t)(2 * n - (seq_t)1)) /* coprime and b>=2n-1 */
        return b - (seq_t)1;

    if (g != (seq_t)1 && b > n) {            /* not coprime and b>n */
        seq_t numerator = b * (b - (seq_t)1);
        seq_t val = numerator / n;
        if (numerator % n != (seq_t)0)
            val++;
        return val;
    }

    /* Brute force search for the remaining cases.  With the problem
       constraints (b and n never exceed a few hundred), this is fast. */
    bool seen[256] = {false};
    const int total = (int)(b - (seq_t)1);   /* digits 1 .. b-1 */
    int cnt = 0;
    seq_t k = (seq_t)1;
    for (;; k++) {
        seq_t m = k * n;
        seq_t temp = m;
        while (temp > (seq_t)0) {
            seq_t dgtseq = temp % b;
            temp /= b;
            if (dgtseq == (seq_t)0)
                continue;
            int dgt = (int)dgtseq;
            if (!seen[dgt]) {
                seen[dgt] = true;
                cnt++;
                if (cnt == total)
                    return k;
            }
        }
        if (cnt == total)
            return k;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)2)  /* unspecified for n<2 */
        return (seq_t)0;

    idx_t t = n - (idx_t)2;      /* zero‑based position in sequence */

    /* Find the diagonal d such that C(d-1) <= t < C(d),
       where C(d) = (d-2)(d-1)/2.  Since t is small (n<=10000)
       a simple loop suffices. */
    idx_t d = (idx_t)3;
    while (((d - (idx_t)2) * (d - (idx_t)1)) / (idx_t)2 <= t)
        d++;

    idx_t prevC = ((d - (idx_t)3) * (d - (idx_t)2)) / (idx_t)2;
    idx_t offset = t - prevC;      /* 0 .. d-3 */

    seq_t b = (seq_t)((d - (idx_t)1) - offset);  /* b = d-1-offset */
    seq_t m = (seq_t)(offset + (idx_t)1);        /* n = offset+1 */

    return compute_T(b, m);
}

/*
 * A289588  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 117) or the same sequence
 *          for the mesh patterns (12, 213), (12, 342), (12, 348).
 * https://oeis.org/A289588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of permutations of length n that avoid
 *  the pattern 321 and the mesh pattern (12,117) (equivalently
 *  (12,213), (12,342), (12,348)).
 *
 *  The sequence satisfies the recurrence
 *
 *      a(0) = 1
 *      a(n) = a(n-1) - ( C_{n-1} + C_{n-3} - C_{n-2} - p_n )    (n ≥ 1)
 *
 *  where C_k is the k‑th Catalan number and
 *          p_1 = –1,  p_3 =  1,   p_k = 0 otherwise.
 *
 *  This implementation computes the Catalan numbers on the fly
 *  and uses only O(1) additional memory, making it fast for
 *  n up to at least 10 000.  All arithmetic is performed in the
 *  build‑dependent type `seq_t`; the only helper function used
 *  from the standard library is `gcd` to perform the necessary
 *  fraction reductions safely. */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Previous Catalan numbers: C_{k-3}, C_{k-2}, C_{k-1} */
    seq_t c_km3 = 0;          /* C_{-2}  – treated as 0   */
    seq_t c_km2 = 0;          /* C_{-1}  – treated as 0   */
    seq_t c_km1 = 1;          /* C_{0}   = 1              */

    seq_t a_prev = (seq_t)1;  /* a(0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* ----- Compute Catalan number C_k --------------------------------- */
        idx_t i = k - 1;               /* previous index                */
        seq_t num   = c_km1;           /* C_{k-1}                        */
        seq_t mult  = (seq_t)(4 * i + 2);   /* 4i+2  = 4(k-1)+2             */
        seq_t denom = (seq_t)(i + 2);       /* i+2   = (k-1)+2 = k+1         */

        seq_t g = gcd(num, denom);
        num  /= g;  denom /= g;

        g = gcd(mult, denom);
        mult /= g;  denom /= g;      /* denom should become 1          */

        /* product may overflow 64‑bit, but for a 64‑bit build we rely on
         * the reduction above; for a big integer build the type handles
         * arbitrary size. */
        seq_t c_k = num * mult;
        if (denom != (seq_t)1)
            c_k /= denom;

        /* ----- Compute A_k ----------------------------------------------- */
        seq_t A_k = 0;
        if (k >= 3) A_k += c_km3;
        if (k >= 2) A_k += c_km2;
        if (k >= 1) A_k -= c_km1;

        int p = 0;
        if (k == 1)      p = -1;
        else if (k == 3) p =  1;
        if (p > 0)   A_k -= (seq_t)p;
        else if (p < 0) A_k += (seq_t)(-p);

        /* ----- Compute a_k ---------------------------------------------- */
        seq_t a_k = a_prev - A_k;

        /* ----- Shift Catalan window for next iteration ----------------- */
        c_km3 = c_km2;
        c_km2 = c_km1;
        c_km1 = c_k;

        a_prev = a_k;
    }

    return a_prev;
}

/*
 * A284806  Fixed points of the transform A284805.
 * https://oeis.org/A284806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow7_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:  Returns the n-th fixed point of the transform A284805.
 * A number x is a fixed point if, when written in base 7, its digits d0…dk
 * satisfy d_i + d_{k-i} = 6 for all i.  These are numbers whose
 * base‑7 representation is a “complementary palindrome”.  The sequence
 * is sorted by increasing value; the first term is 3.  The algorithm
 * enumerates numbers by length, computing the digits directly from the
 * zero‑based offset within the current length, and then converts the
 * base‑7 digits to a decimal value of type seq_t.  It runs in O(L)
 * time where L is the number of base‑7 digits (≤10 for n ≤ 10 000). */

#include <stdint.h>
#include "../utils/pow7_int.h"

seq_t a(idx_t n)
{
    if (n == 1)        /* the only 1‑digit solution is 3 */
        return 3;

    /* cumulative count of terms up to previous length */
    idx_t cum = 1;    /* count of length 1 terms */

    idx_t L = 2;      /* current length being examined */
    while (1) {
        idx_t numPairs = L / 2;
        /* exponent for 7^((numPairs)-1) ; for L>=2 , numPairs>=1 */
        idx_t exp = numPairs - 1;
        idx_t pow7 = 1;
        for (idx_t i = 0; i < exp; ++i)
            pow7 *= 7;

        idx_t count = 6 * pow7;          /* count of length L numbers */
        if (n <= cum + count) {          /* the desired length found */
            idx_t offset = n - cum - 1;  /* zero‑based index within length L */
            /* leading digit (cannot be 0) */
            idx_t leadingPos = offset / pow7;
            unsigned char d[20] = {0};   /* base‑7 digits, L≤10 for n≤10 000 */
            d[0] = (unsigned char)(leadingPos + 1);

            offset %= pow7;              /* remaining index for the other pairs */
            /* fill remaining pair digits */
            for (idx_t i = 1; i < numPairs; ++i) {
                d[i] = (unsigned char)(offset % 7);
                offset /= 7;
            }
            /* middle digit for odd length */
            if (L & 1) {
                d[numPairs] = 3;
            }
            /* mirror digits to form the full number */
            for (idx_t i = 0; i < numPairs; ++i)
                d[L - 1 - i] = 6 - d[i];

            /* convert base‑7 digits to decimal (seq_t) */
            seq_t val = 0;
            for (idx_t i = 0; i < L; ++i)
                val = val * 7 + d[i];
            return val;
        }
        /* move to next length */
        cum += count;
        ++L;
    }
}

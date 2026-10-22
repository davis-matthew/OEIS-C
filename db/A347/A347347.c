/*
 * A347347  If n is odd, a(n) is the smallest positive integer that is a
 *          palindrome when represented in binary, and is made by appending
 *          some number (zero or more) of binary digits on the left of n's
 *          binary representation. If n is even, then a(n) = 0.
 * https://oeis.org/A347347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest binary palindrome whose rightmost bits equal the
 *            binary representation of n, if n is odd; a(n)=0 otherwise
 * Range:     u8 n <= 40, u16 n <= 640, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A succinct algorithm: for an odd n with k binary digits,
   try palindrome lengths L = k, k+1, …, 2k.
   For each L, set the last k bits to n’s digits,
   propagate the palindrome constraint p[i] = p[L-1-i].
   If no conflict and the leading bit is 1, that palindrome is the
   minimal one.  The bound L ≤ 2k always works. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if ((n & 1) == 0)           /* even n -> 0 */
        return 0;

    /* number of bits in n (n ≥ 1) */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) { tmp >>= 1; ++k; }

    /* bits array reused for each candidate length; max required is 128 */
    /* Use a small stack array, initialized per L. */
    int bits[128];

    for (idx_t L = k; L <= 2 * k; ++L)
    {
        /* initialise all positions as unknown (-1) */
        for (idx_t i = 0; i < L; ++i)
            bits[i] = -1;

        /* set the suffix bits to match n (most‑significant first) */
        for (idx_t j = 0; j < k; ++j)
            bits[L - k + j] = (int)((n >> (k - 1 - j)) & 1);

        /* propagate palindrome symmetry */
        int conflict = 0;
        for (idx_t i = 0; i < L; ++i)
        {
            idx_t j = L - 1 - i;
            if (bits[i] != -1 && bits[j] == -1)
                bits[j] = bits[i];
            else if (bits[i] != -1 && bits[j] != -1
                     && bits[i] != bits[j])
            {
                conflict = 1;
                break;
            }
        }
        if (conflict) continue;

        /* leading bit must be 1 (guaranteed by n’s LSB = 1,
           but enforce just in case) */
        if (bits[0] == 0) continue;      /* leading zero – invalid */
        if (bits[0] == -1) bits[0] = 1;   /* safety for degenerate case */

        /* construct the resulting integer */
        seq_t val = 0;
        for (idx_t i = 0; i < L; ++i)
            val = (val << 1) | (seq_t)bits[i];

        return val;
    }

    /* Should never reach here for odd n */
    return 0;
}

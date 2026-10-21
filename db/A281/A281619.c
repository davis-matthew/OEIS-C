/*
 * A281619  Integer values of (A000045^2-1)/9 where A000045(m) is the m-th
 *          Fibonacci number.
 * https://oeis.org/A281619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (fib(k)^2 - 1) / 9 * * Here fib(m) returns A000045(m) –
 *            the m‑th Fibonacci number with * F(0)=0, F(1)=1. The index k is
 *            the n‑th integer whose remainder * modulo 12 belongs to the set
 *            {1,6,10,11,2}. The first four terms * use the residues
 *            1,6,10,11; after that the pattern 1,2,6,10,11 * rep...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the first four indices explicitly.  These map to
     * Fibonacci indices 1, 6, 10, and 11. */
    if (n == 1) return 0;
    if (n == 2) return 7;
    if (n == 3) return 336;
    if (n == 4) return 880;

    /* For n >= 5, the residues modulo 12 repeat the pattern:
     * 1, 2, 6, 10, 11.  The first group after n=4 starts at k=13.
     */
    static const uint8_t pattern[5] = {1, 2, 6, 10, 11};

    /* Compute k = 12 * (group + 1) + pattern[offset] */
    idx_t m      = n - 5;               /* offset after the first 4 terms */
    idx_t group  = m / 5;               /* which repetition we are in */
    idx_t offset = m % 5;               /* position within the group   */
    idx_t k      = 12 * (group + 1) + pattern[offset];

    /* Evaluate (fib(k)^2 - 1) / 9 safely. */
    seq_t f   = fib(k);          /* F(k)               */
    seq_t fdiv = f / 9;          /* floor(f/9)         */
    seq_t fmod = f % 9;          /* remainder          */

    /* Use the identity (f^2 - 1)/9 = 9*(fdiv)^2 + 2*fdiv*fmod + (fmod^2 - 1)/9
     * which avoids overflow on 64‑bit seq_t. */
    seq_t t1   = fdiv * fdiv;             /* (fdiv)^2            */
    seq_t t2   = fdiv * fmod;             /* fdiv * fmod         */
    seq_t t3   = (fmod * fmod - 1) / 9;   /* (fmod^2 - 1)/9     */

    return 9 * t1 + 2 * t2 + t3;
}

/*
 * A266596  Total number of OFF (white) cells after n iterations of the "Rule
 *          37" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+4*n+(-1)^n*(n-3)-3)/2 for n>0, a(0)=0
 * Range:     u8 n <= 21, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use 128‑bit arithmetic to avoid overflow for large n */
    __int128 nn   = n;                 /* copy n into signed 128 bits */
    __int128 num  = nn * nn + 4 * nn - 3;

    /* Add or subtract (n-3) depending on the parity of n. */
    if (n & 1)            /* n odd: subtract (n-3) = (nn-3) */
        num -= (nn - 3);
    else                  /* n even: add (n-3) */
        num += (nn - 3);

    return (seq_t)(num / 2);           /* division is exact (integer)  */
}

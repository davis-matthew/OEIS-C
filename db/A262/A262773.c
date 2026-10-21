/*
 * A262773  A Beatty sequence: a(n)=floor(q*n) where q=A231187.
 * https://oeis.org/A262773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + 2*cos(2π/7)) * n)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Beatty sequence set by the longer diagonal of a regular heptagon
 * (A262773).  The sequence has a constant difference pattern of
 * 2,2,2,3 repeated after the fourth term.  This implementation
 * uses that observation to compute a(n) in O(1) time for any n. */
seq_t a(idx_t n)
{
    /* Base cases (offset 0) */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)8;

    /* For n >= 5, a(n) = 8 + 9*floor((n-4)/4) + {0,3,5,7}[ (n-4)%4 ]   */
    idx_t m   = n - 4;                /* number of differences after a(4) */
    idx_t q4  = m / 4;               /* number of full 4‑term cycles   */
    idx_t r   = m % 4;               /* remaining terms in the partial cycle */

    seq_t sum = (seq_t)q4 * (seq_t)9;  /* 9 = 3 + 2 + 2 + 2 */

    /* Add the contribution of the incomplete cycle, if any */
    switch (r) {
        case 0: /* no extra terms */ break;
        case 1: sum += (seq_t)3; break;  /* diff sequence: 3,2,2,2 */
        case 2: sum += (seq_t)5; break;  /* 3 + 2 */
        case 3: sum += (seq_t)7; break;  /* 3 + 2 + 2 */
    }

    return (seq_t)8 + sum;
}

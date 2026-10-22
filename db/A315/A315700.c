/*
 * A315700  Coordination sequence Gal.6.638.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence for Gal.6.638.4:
   a(0)=1; for n≥1, a(n)=a(n‑1)+c(n) where
   c(n)=5 if (n==1) or any of the following hold:
        n%6==2 and ((n/6)&1)==1     (odd block)
        n%6==3 and ((n/6)&1)==0     (even block)
        n%6==4 and ((n/6)&1)==1     (odd block)
        n%6==5 and ((n/6)&1)==0     (even block)
   otherwise c(n)=6.  */
seq_t a(idx_t n)
{
    seq_t result = 1;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t inc;
        idx_t blk = i / 6;
        int r = (int)(i % 6);
        if (r == 1)                    /* i≡1 (mod 6) */
        {
            if (i == 1)
                inc = 5;
            else
                inc = 6;
        }
        else if (r == 2)               /* i≡2 (mod 6) */
            inc = (blk & 1) ? 5 : 6;
        else if (r == 3)               /* i≡3 (mod 6) */
            inc = ((blk & 1) == 0) ? 5 : 6;
        else if (r == 4)               /* i≡4 (mod 6) */
            inc = (blk & 1) ? 5 : 6;
        else if (r == 5)               /* i≡5 (mod 6) */
            inc = ((blk & 1) == 0) ? 5 : 6;
        else                            /* r == 0 (i≡0 mod 6) */
            inc = 6;
        result += inc;
    }
    return result;
}

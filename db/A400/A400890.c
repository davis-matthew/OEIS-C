/*
 * A400890  Numbers k such that the 2-adic valuation of 3*k+2 is odd.
 * https://oeis.org/A400890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = the nth non‑negative even integer k for which
 * the 2‑adic valuation of (3*k + 2) is odd.
 * That is equivalent to: let k = 2*m, then v2(3*m + 1) is even.
 */
seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 0 */
    if (n < 1)
        return 0; /* undefined, but safe fallback */

    idx_t count = 0;             /* how many suitable k found */
    seq_t  t = 1;                /* 3*m + 1 for m = 0 */

    for (idx_t m = 0; ; ++m, t += (seq_t)3)
    {
        seq_t tmp = t;
        int    tv  = 0;          /* exponent of 2 in (3*m+1) */

        while ((tmp & 1) == 0)
        {
            ++tv;
            tmp >>= 1;
        }

        /* v2(3*m+1) even  --> add k = 2*m to the sequence */
        if ((tv & 1) == 0)
        {
            ++count;
            if (count == n)
                return (seq_t)2 * (seq_t)m;
        }
    }
}

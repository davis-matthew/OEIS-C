/*
 * A363058  Number of ways to get n points in a bridge hand.
 * https://oeis.org/A363058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = # of solutions to a+2b+3c+4d = n with 0 ≤ a,b,c,d ≤ 4 and
 *            a+b+c+d ≤ 13 (bridge hand point totals)
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Iterate over all possible counts of 4‑point (A), 3‑point (K),
       2‑point (Q) and 1‑point (J) cards. */
    for (idx_t d = 0; d <= 4; ++d)          /* aces */
    {
        for (idx_t c = 0; c <= 4; ++c)      /* kings */
        {
            for (idx_t b = 0; b <= 4; ++b)  /* queens */
            {
                for (idx_t a_card = 0; a_card <= 4; ++a_card)   /* jacks */
                {
                    idx_t score = a_card + 2*b + 3*c + 4*d;
                    if (score == n && (a_card + b + c + d) <= 13)
                        ++count;
                }
            }
        }
    }
    return count;
}

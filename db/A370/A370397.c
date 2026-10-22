/*
 * A370397  a(n) = floor(g(n + 1/4)), where g(x) is the function defined for
 *          all real numbers except the negative integers by the recurrence
 *          formula g(x+1) = g(x)*(x+1), with g(x) = 1 for 0 <= x < 1.
 * https://oeis.org/A370397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((Product_{k=1..n} (4*k + 1))/4^n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For built‑in integer seq_t (up to 128 bits) use a 128‑bit intermediate
       to prevent overflow for n ≤ 24 (all terms that fit in 64 bits).  For
       arbitrary‑precision seq_t types use seq_t arithmetic directly. */
    if (sizeof(seq_t) <= 16) {
        unsigned __int128 num = 1;
        unsigned __int128 denom = 1;
        for (idx_t k = 1; k <= n; ++k) {
            num *= (unsigned __int128)(4 * k + 1);
            denom *= (unsigned __int128)4;
        }
        return (seq_t)(num / denom);
    } else {
        seq_t num = 1;
        seq_t denom = 1;
        for (idx_t k = 1; k <= n; ++k) {
            num *= (seq_t)(4 * k + 1);
            denom *= (seq_t)4;
        }
        return num / denom;   /* integer division gives the floor */
    }
}

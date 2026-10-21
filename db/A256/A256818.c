/*
 * A256818  Number of length n+3 0..1 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A256818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 10*n^4 + 155*n^3 - 310*n^2 + 1584*n + 480) / 120
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=16, a(2)=32, a(3)=64, … (OEIS A256818) */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       (the offset is 1, this value is never used). */
    if (n == 0) return 0;

    /* Use a 128‑bit signed intermediate to avoid overflow for n
       up to the limits of practical use (e.g. n <= 10⁶ for 64‑bit seq_t). */
    unsigned __int128 nn  = (unsigned __int128) n;
    unsigned __int128 n2  = nn * nn;
    unsigned __int128 n3  = n2 * nn;
    unsigned __int128 n4  = n3 * nn;
    unsigned __int128 n5  = n4 * nn;

    __int128 num = ( __int128) n5
                 + ( __int128)(10 * n4)
                 + ( __int128)(155 * n3)
                 - ( __int128)(310 * n2)
                 + ( __int128)(1584 * nn)
                 + 480;

    return (seq_t)(num / 120);
}

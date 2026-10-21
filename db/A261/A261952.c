/*
 * A261952  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable vertex of the
 *          triangles of the (n-1)-th generation (this is the "vertex to
 *          vertex" version); for the even n-th generation use the "side to
 *          side" version; a(n) is the number of triangles...
 * https://oeis.org/A261952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(1-(-1)^n+6*n)/4 for n≥4 Initial terms: a(0)=1, a(1)=3,
 *            a(2)=9, a(3)=18
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)18;

    /* Use the closed‑form expression for n ≥ 4 */
    seq_t ns = (seq_t)n;                    /* cast index to seq_t */
    seq_t odd_term = (n & 1) ? (seq_t)2 : (seq_t)0;   /* 2 if n odd, else 0 */
    seq_t term = 6 * ns + odd_term;         /* 6*n + (n odd?2:0) */
    term = term * (seq_t)3;                 /* multiply by 3 */
    term = term / (seq_t)4;                 /* divide by 4 to get a(n) */
    return term;
}

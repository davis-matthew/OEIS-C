/*
 * A399430  b * c * d where b >= c >= d >= 0 ordered by b then c then d.
 * https://oeis.org/A399430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = A056556(n) * A056557(n) * A056558(n)
 * Range:     u8 n <= 118, u16 n <= 9433, u32 n <= 9433, u64 n <= 9433,
 *            u128 n <= 9433, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the nth product b*c*d where b >= c >= d >= 0, lexicographically
   ordered by (b,c,d).  Parameter n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* out of bounds, but safe */

    /* Find the minimal b with cumulative count >= n. */
    idx_t b = 0;
    for (;;) {
        /* cumulative count T(b) = (b(b+1)(b+5)+6b+6)/6 */
        unsigned __int128 num = (unsigned __int128)b * (b + 1) * (b + 5) + 6 * b + 6;
        idx_t cum = (idx_t)(num / 6);
        if (cum >= n)
            break;
        ++b;
    }

    /* Cumulative count up to b-1 */
    idx_t cumprev = 0;
    if (b > 0) {
        unsigned __int128 numprev = (unsigned __int128)(b - 1) * b * (b + 4) + 6 * (b - 1) + 6;
        cumprev = (idx_t)(numprev / 6);
    }

    /* 0‑based index within the block for this b */
    idx_t r = n - cumprev - 1;

    /* Find c such that (c+1)(c+2)/2 > r  */
    idx_t c = 0;
    for (;;) {
        unsigned __int128 bound = (unsigned __int128)(c + 1) * (c + 2) / 2;
        if (bound > r)
            break;
        ++c;
    }

    /* Number of tuples before current (c,d) pair */
    unsigned __int128 prior = (unsigned __int128)c * (c + 1) / 2;
    idx_t d = (idx_t)(r - prior);   /* 0‑based within this c */

    /* Compute the product */
    seq_t sb = (seq_t)b;
    seq_t sc = (seq_t)c;
    seq_t sd = (seq_t)d;
    return sb * sc * sd;
}

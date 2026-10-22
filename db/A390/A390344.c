/*
 * A390344  Upper (1/3)-midsequence of binomial(n,3) and binomial(n,2); see
 *          Comments.
 * https://oeis.org/A390344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n^3 - n)/18)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses 128‑bit intermediate to avoid overflow. */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the result is 0; the general formula also yields 0. */
    unsigned __int128 num = (unsigned __int128)n * n * n - n; /* n^3 - n  */
    num += 17;                     /* add (18‑1) to perform ceil division */
    num /= 18;                     /* integer division now gives ceil */
    return (seq_t)num;
}

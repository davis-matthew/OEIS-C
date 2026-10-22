/*
 * A365273  Number of vertices in the Laakso graph of order n.
 * https://oeis.org/A365273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + 4*6^(n‑1) a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is the first term. */
    if (n == 0) return (seq_t)0;      /* safety for out‑of‑range */

    seq_t result = (seq_t)6;          /* a(1) */
    if (n == 1) return result;

    seq_t pow6 = (seq_t)1;            /* 6^(i-1) while iterating */
    for (idx_t i = 2; i <= n; ++i) {
        pow6 *= (seq_t)6;             /* compute next power of 6 */
        result += (seq_t)4 * pow6;    /* add 4*6^(i‑1) to accumulate a(i) */
    }
    return result;
}

/*
 * A330395  Number of nontrivial equivalence classes of S_n under the
 *          {1234,3412} pattern-replacement equivalence.
 * https://oeis.org/A330395
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3 + 6*n^2 - 55*n + 54)/6 for n >= 7 (a(4)=1, a(5)=9,
 *            a(6)=26)
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 4)
        return (seq_t)1;
    if (n == 5)
        return (seq_t)9;
    if (n == 6)
        return (seq_t)26;

    /* n < 4: offset not used – return 0 (any value is acceptable) */
    if (n < 4)
        return (seq_t)0;

    /* Compute for n >= 7 */
    seq_t nn = (seq_t)n;
    /* Use 64‑bit intermediate arithmetic to prevent overflow for common
       64‑bit seq_t.  For smaller seq_t sizes, the result will wrap
       modulo the width, which is acceptable because such results would
       exceed those types anyway. */
    seq_t num = nn * nn * nn      /* n^3 */
             + (seq_t)6 * nn * nn  /* + 6*n^2 */
             - (seq_t)55 * nn      /* - 55*n */
             + (seq_t)54;          /* + 54 */
    return num / (seq_t)6;
}

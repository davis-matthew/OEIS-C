/*
 * A254285  Hexagonal numbers (A000384) which are also centered triangular
 *          numbers (A005448).
 * https://oeis.org/A254285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + 37634*a(n‑2) – 37634*a(n‑3) – a(n‑4) + a(n‑5)
 *            Initial values: a(1) = 1 a(2) = 1891 a(3) = 26335 a(4) =
 *            71156485 a(5) = 991081981 The recursive definition is used for n
 *            > 5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle undefined indices gracefully. */
    if (n < 1)
        return (seq_t)0;

    /* Return the first five terms directly. */
    if (n == 1) return (seq_t)1ULL;
    if (n == 2) return (seq_t)1891ULL;
    if (n == 3) return (seq_t)26335ULL;
    if (n == 4) return (seq_t)71156485ULL;
    if (n == 5) return (seq_t)991081981ULL;

    /* Iteratively compute up to the requested index using only five
       sliding variables; no dynamic memory required.                         */
    seq_t a1 = (seq_t)1ULL;
    seq_t a2 = (seq_t)1891ULL;
    seq_t a3 = (seq_t)26335ULL;
    seq_t a4 = (seq_t)71156485ULL;
    seq_t a5 = (seq_t)991081981ULL;

    idx_t i;
    for (i = 6; i <= n; ++i) {
        seq_t next = a5
                   + ((seq_t)37634 * a4)
                   - ((seq_t)37634 * a3)
                   - a2
                   + a1;
        /* Shift the window of previous values. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}

/*
 * A287194  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 259", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 1; a(1) = 11; for odd n≥3, a(n) is the decimal * number
 *            consisting of (n‑1) ones followed by two zeros
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)      /* even n */
        return (seq_t)1;

    if (n == (idx_t)1)            /* n = 1 */
        return (seq_t)11;

    /* odd n ≥ 3: build n‑1 ones, then append two zeros */
    seq_t val = 0;
    for (idx_t i = 0; i < n - 1; ++i)
        val = val * (seq_t)10 + (seq_t)1;
    val = val * (seq_t)100;
    return val;
}

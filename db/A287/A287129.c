/*
 * A287129  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 245", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 10, a(2) = 0, a(3) = 1110, a(4) = 11, a(5) =
 *            111110, a(n) = 11 for n > 5 and even, a(n) = 10*(10^n - 10) / 9
 *            for n > 5 and odd
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Explicit small-index values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1110;
    if (n == 4) return (seq_t)11;
    if (n == 5) return (seq_t)111110;

    /* For n > 5 the behavior depends on parity. */
    if ((n & 1) == 0)          /* even */
        return (seq_t)11;

    /* Odd case: a(n) = repunit(n‑1) * 100.
       Compute the repunit iteratively: repunit(L) = repunit(L-1) * 10 + 1.
       This keeps intermediate values as small as possible and works
       correctly for 64‑bit builds up to the largest n whose result fits
       in uint64_t.  For larger n the value does not fit anyway. */
    seq_t rep = (seq_t)1;          /* repunit(1) */
    for (idx_t i = 1; i < n - 1; ++i)
        rep = rep * (seq_t)10 + (seq_t)1;

    return rep * (seq_t)100;       /* append two zeros */
}

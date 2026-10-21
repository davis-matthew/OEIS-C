/*
 * A285648  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 73", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0,1 * a(2) = 100 * a(3) = 11 * a(n) = 0 if n >
 *            3 and n is even * a(n) = (10^(n+1) – 1) / 9 if n > 3 and n is
 *            odd * The odd‐index case is implemented by constructing the
 *            repunit * consisting of n+1 decimal digits ‘1’, avoiding any
 *            intermediate * power‑of‑10 calculation that could...
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)          return 1;
    if (n == 2)                    return 100;
    if (n == 3)                    return 11;

    if ((n & 1u) == 0)             return 0;   /* even index > 3 */

    /* odd index: build the repunit of length n+1
     * (10^(n+1) – 1) / 9 = 111...1  (n+1 ones)   */
    seq_t result = 0;
    idx_t len = n + 1;
    for (idx_t i = 0; i < len; ++i)
        result = result * 10 + 1;

    return result;
}

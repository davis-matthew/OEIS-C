/*
 * A284479  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 950", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = decimal value of the binary string obtained from the left
 *            edge to the * origin in state n of Rule 950. The binary string
 *            is: * n = 0 : "1" * n even and n ≥ 2 : "10" followed by (n-1)
 *            '1' bits * n odd, n ≡ 1 (mod 4) : all '1' bits of length n+1 * n
 *            odd, n ≡ 3 (mod 4) : * n = 3 : "1111" * n...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)        /* n = 0 : single cell */
        return (seq_t)1;

    /* Even indices (n ≥ 2) ------------------------------------------- */
    if ((n & 1u) == 0) {
        seq_t v = 0;
        /* first bit 1 */
        v = v * (seq_t)10 + (seq_t)1;
        /* second bit 0 */
        v = v * (seq_t)10 + (seq_t)0;
        /* remaining (n-1) bits are 1 */
        for (idx_t i = 0; i < n - 1; ++i)
            v = v * (seq_t)10 + (seq_t)1;
        return v;
    }

    /* Odd indices ---------------------------------------------- */
    /* Handle the special case n = 3 */
    if (n == 3u) {
        seq_t v = 0;
        for (idx_t i = 0; i < 4; ++i)
            v = v * (seq_t)10 + (seq_t)1;
        return v;
    }

    /* n ≡ 1 (mod 4)  -> all 1’s */
    if ((n % 4u) == 1u) {
        seq_t v = 0;
        for (idx_t i = 0; i < n + 1; ++i)
            v = v * (seq_t)10 + (seq_t)1;
        return v;
    }

    /* n ≡ 3 (mod 4) and n ≥ 7  -> “1110” followed by (n-3) 1’s */
    {
        seq_t v = 0;
        /* three leading 1’s */
        v = v * (seq_t)10 + (seq_t)1;
        v = v * (seq_t)10 + (seq_t)1;
        v = v * (seq_t)10 + (seq_t)1;
        /* a single 0 */
        v = v * (seq_t)10 + (seq_t)0;
        /* remaining (n-3) bits are 1 */
        for (idx_t i = 0; i < n - 3; ++i)
            v = v * (seq_t)10 + (seq_t)1;
        return v;
    }
}

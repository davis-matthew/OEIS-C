/*
 * A309576  Table read by rows: T(n, k) is the last k bits of n interpreted as
 *          a base-2 representation of a number and converted to decimal, 0 <=
 *          k <= A070939(n).
 * https://oeis.org/A309576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base, look
 * Formula:   Let f(r)=⌊log₂ r⌋+2. For a given n, find the unique integer r
 *            with ∑_{i=1}^{r} f(i) ≥ n. Let k = n−∑_{i=1}^{r−1} f(i)−1. Then
 *            a(n) = r mod 2ᵏ (with the convention 2⁰=1, so r mod 1 = 0)
 * Range:     u8 n <= 2057, u16 n <= 9987, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = 0;
    unsigned __int128 cum = 0;
    const unsigned int seq_bits = sizeof(seq_t) * 8;

    for (;;)
    {
        ++r;                                   /* next row number   */
        idx_t bits = 64 - __builtin_clzll(r);   /* number of bits in r */
        idx_t f = bits + 1;                     /* entries in this row */
        cum += f;

        if (cum >= n) {
            unsigned __int128 before = cum - f;
            unsigned __int128 offset = (unsigned __int128)n - before - 1;
            idx_t k = (idx_t)offset;             /* column index (0‑based) */

            /* compute r mod 2^k */
            seq_t mask;
            if (k == 0)
                mask = 0;
            else if (k >= seq_bits)
                mask = ~((seq_t)0);
            else
                mask = ((seq_t)1 << k) - 1;

            return ((seq_t)r) & mask;
        }
    }
}

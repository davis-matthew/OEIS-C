/*
 * A387011  a(n) = Sum_{k=0..n} binomial(4*n+4,k).
 * https://oeis.org/A387011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+4, k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Working value for 4*n+4; use 128‑bit to avoid overflow. */
    unsigned __int128 m = (unsigned __int128)4 * n + 4;

    seq_t sum = 0;      /* running total */
    seq_t bin = 1;      /* current binomial coefficient C(m, k) */

    for (idx_t k = 0; k <= n; ++k)
    {
#ifdef __cplusplus
        /* Big‑integer build (C++17). Use the library's operators. */
        if (k > 0)
            bin = (bin * (seq_t)(m - k + 1)) / k;
#else
        /* Numeric build (C). Use 128‑bit arithmetic to avoid overflow. */
        if (k > 0)
        {
            unsigned __int128 temp = (unsigned __int128)bin * (m - k + 1);
            bin = (seq_t)(temp / k);
        }
#endif
        sum += bin;
    }
    return sum;
}

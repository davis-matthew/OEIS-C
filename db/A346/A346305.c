/*
 * A346305  Positions of words in A076478 that start with 1 and end with 1.
 * https://oeis.org/A346305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Let L be the smallest integer with 2^{L-1} ≥ n. * For n = 1,
 *            a(n) = 2. * For n > 1, let k = n - 2^{L-2}. Then * a(n) = 2^L +
 *            2^(L-1) + 2·(k-1)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset does not matter for n < 1 */
    /* Find L such that 2^(L-1) ≥ n */
    idx_t L = 1;
    idx_t cum = 1;                    /* cum = 2^(L-1) */
    while (cum < n) {
        ++L;
        cum <<= 1;
    }
    if (L == 1) {
        return (seq_t)2;
    }
    idx_t cum_before = cum >> 1;      /* 2^(L-2) */
    idx_t k = n - cum_before;         /* 1‑based offset within this length */

    seq_t two_pow_L   = ((seq_t)1) << L;     /* 2^L */
    seq_t two_pow_L_1 = ((seq_t)1) << (L-1); /* 2^(L-1) */
    seq_t result = two_pow_L + two_pow_L_1 + ((seq_t)2) * ((seq_t)(k - 1));
    return result;
}

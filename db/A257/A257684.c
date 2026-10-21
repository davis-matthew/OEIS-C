/*
 * A257684  Discard the rightmost digit from the factorial base representation
 *          of n and subtract one from all remaining nonzero digits, then
 *          convert back to decimal.
 * https://oeis.org/A257684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   discard the rightmost digit from the factorial base
 *            representation of n, subtract one from all remaining nonzero
 *            digits, and convert back to decimal
 * Range:     u8 n <= 2249, u16 n <= 9774, u32 n <= 9774, u64 n <= 9774,
 *            u128 n <= 9774, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* precompute factorials up to 20 (fits in idx_t). */
    static idx_t fact[21];
    static int fact_init = 0;
    if (!fact_init) {
        fact[0] = 1;
        for (int i = 1; i <= 20; ++i) {
            fact[i] = fact[i-1] * (idx_t)i;
        }
        fact_init = 1;
    }

    unsigned digits[21] = {0};
    idx_t m = n;
    for (int i = 20; i >= 1; --i) {
        digits[i] = (unsigned)(m / fact[i]);
        m %= fact[i];
    }

    seq_t result = 0;
    seq_t factor = 1;      /* represents (i-1)! for the current i */
    for (int i = 2; i <= 20; ++i) {
        seq_t d = (seq_t)digits[i];
        if (d > 0) --d;        /* subtract 1 from non‑zero digits */
        result += d * factor;
        factor *= (seq_t)i;    /* advance to next factorial (i! for next i+1) */
    }
    return result;
}

/*
 * A255766  Partial sums of A255745.
 * https://oeis.org/A255766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 11 * sum_{m=1}^{n-1} 10^{popcount(m)-1} for n ≥ 1
 *            where popcount(m) is the number of 1‑bits in the binary
 *            representation of m. This implements the partial sums of
 *            A255745, the first row of the square array A255741. The sequence
 *            is computed by summing 10^{popcount(m)-1} for 1 ≤ m < n, m...
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 767, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, behaviour for n = 0 is arbitrary */
        return 0;
    if (n == 1)          /* the first term is 1 */
        return 1;

    /* Pre‑compute powers of 10 up to 10^12 (popcount(9999) = 13, so exponent ≤ 12) */
    static seq_t pow10[20] = {1};
    static int initialized = 0;
    if (!initialized) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 20; ++i)
            pow10[i] = pow10[i - 1] * ((seq_t)10);
        initialized = 1;
    }

    seq_t sum = 0;
    for (idx_t m = 1; m < n; ++m) {
        /* popcount of a 64‑bit integer */
        unsigned int pc = __builtin_popcountll((unsigned long long)m);
        /* popcount(m) ≥ 1 for m ≥ 1, so pc-1 ≥ 0 */
        sum += pow10[pc - 1];
    }

    /* a(n) = 1 + 11 * sum */
    sum = sum * ((seq_t)11) + ((seq_t)1);
    return sum;
}

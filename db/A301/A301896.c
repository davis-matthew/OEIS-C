/*
 * A301896  a(n) = product of total number of 0's and total number of 1's in
 *          binary expansions of 0, ..., n.
 * https://oeis.org/A301896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (total number of 0's in binary expansions of 0..n) *
 *            (total number of 1's in binary expansions of 0..n)
 * Range:     u8 n <= 10, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t zeros = 0;
    seq_t ones  = 0;

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {             /* binary representation of 0 is "0" */
            zeros += 1;
            continue;
        }
        unsigned long long x = i;
        unsigned int pop  = __builtin_popcountll(x);          /* number of 1's */
        unsigned int bits = 64 - __builtin_clzll(x);           /* number of bits */
        zeros += (seq_t)bits - (seq_t)pop;
        ones  += (seq_t)pop;
    }
    return zeros * ones;
}

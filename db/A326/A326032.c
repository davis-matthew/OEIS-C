/*
 * A326032  a(2^x + ... + 2^z) = w(x) + ... + w(z), where x...z are distinct
 *          nonnegative integers and w = A000120.
 * https://oeis.org/A326032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i: bit i set in n} popcount(i)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses builtin popcount. Handles any idx_t and seq_t. */
seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t i = 0;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1ULL) {
            sum += (seq_t)__builtin_popcountll((unsigned long long)i);
        }
        tmp >>= 1;
        ++i;
    }
    return sum;
}

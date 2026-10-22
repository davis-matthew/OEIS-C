/*
 * A302437  a(n) is the number of ways of writing the binary expansion of n as
 *          a concatenation of nonempty substrings with Hamming weight at most
 *          2.
 * https://oeis.org/A302437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): count of ways to split the binary expansion
   of n into non‑empty substrings each having Hamming weight ≤ 2.
   The routine works in O(L²) time, where L is the bit length of n 
   (L ≤ 64 for any index fitting in 64 bits). */
seq_t a(idx_t n) {
    /* handle n == 0 */
    if (n == (idx_t)0) return (seq_t)1;

    /* determine bit length L */
    idx_t temp = n;
    unsigned L = 0;
    while (temp) { temp >>= 1; ++L; }

    /* bits[0..L-1]: most significant to least significant bit */
    unsigned bits[65];            /* L ≤ 64 */
    unsigned ones_prefix[65] = {0};

    /* populate bits and prefix sum of ones */
    for (unsigned i = 0; i < L; ++i) {
        unsigned bit = (n >> (L - 1 - i)) & 1;
        bits[i] = bit;
        ones_prefix[i + 1] = ones_prefix[i] + bit;
    }

    /* dp[i] = number of ways to partition first i bits (i = 0..L) */
    seq_t dp[65] = {0};
    dp[0] = (seq_t)1;

    for (unsigned start = 0; start < L; ++start) {
        seq_t ways_start = dp[start];
        if (ways_start == (seq_t)0) continue;
        for (unsigned end = start + 1; end <= L; ++end) {
            unsigned ones = ones_prefix[end] - ones_prefix[start];
            if (ones <= 2) {
                dp[end] += ways_start;
            }
        }
    }

    return dp[L];
}

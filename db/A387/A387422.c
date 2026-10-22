/*
 * A387422  The length of the maximal common prefix of the binary expansions
 *          of n and sigma(n), where sigma is the sum of divisors function.
 * https://oeis.org/A387422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of maximal common prefix of binary expansions of n
 *            and sigma(n)
 * Range:     u8 n <= 356, u16 n <= 9607, u32 n <= 9607, u64 n <= 9607,
 *            u128 n <= 9607, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the number of bits in the binary representation of v (v > 0). */
static unsigned bitlen_seq_t(seq_t v) {
    unsigned len = 0;
    while (v) {
        v >>= 1;
        ++len;
    }
    return len;
}

/* Compute sigma(n) = sum of all positive divisors of n. */
static seq_t sigma_of_uint64(uint64_t n) {
    uint64_t sum = 1 + n;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += d;
            if (d * d != n)
                sum += n / d;
        }
    }
    return (seq_t)sum;
}

seq_t a(idx_t n) {
    /* n is at least 1 */
    uint64_t n64 = (uint64_t)n;
    seq_t n_seq = (seq_t)n64;
    seq_t sigma = sigma_of_uint64(n64);

    unsigned l1 = bitlen_seq_t(n_seq);
    unsigned l2 = bitlen_seq_t(sigma);

    if (sigma == n_seq)        /* equal numbers */
        return (seq_t)l1;

    unsigned maxlen = l1 > l2 ? l1 : l2;
    unsigned shift1  = maxlen - l1;
    unsigned shift2  = maxlen - l2;

    seq_t padded_n    = n_seq << shift1;
    seq_t padded_sig  = sigma << shift2;
    seq_t x           = padded_n ^ padded_sig;
    unsigned xor_bits = bitlen_seq_t(x);
    unsigned leading_zeros = maxlen - xor_bits;

    /* Limit to the shorter length */
    unsigned min_l = l1 < l2 ? l1 : l2;
    if (leading_zeros > min_l)
        leading_zeros = min_l;

    return (seq_t)leading_zeros;
}

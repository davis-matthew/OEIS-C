/*
 * A387423  The length of binary expansion of n minus the length of the
 *          maximal common prefix of the binary expansions of n and sigma(n),
 *          where sigma is the sum of divisors function.
 * https://oeis.org/A387423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 655, u16 n <= 9466, u32 n <= 9466, u64 n <= 9466,
 *            u128 n <= 9466, big any n
 * Helpers:   floor_log2_uint64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (floor_log2(n)+1) - LCP_len(n, sigma(n))
 * A(n) is the number of bits in the binary expansion of n after
 * removing the longest common prefix with the binary expansion of
 * sigma(n) (sum of divisors of n).  For n>=1.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* work with 64‑bit unsigned integers – for n≤10⁴ σ(n) fits in 64 bits */
    uint64_t v = (uint64_t)n;
    uint64_t sig = (uint64_t)sigma;

    /* length of binary representations */
    unsigned int bits_n   = floor_log2_uint64(v) + 1;
    unsigned int bits_sig = floor_log2_uint64(sig) + 1;

    /* longest common prefix length */
    unsigned int minbits = bits_n < bits_sig ? bits_n : bits_sig;
    unsigned int lcp = 0;

    for (unsigned int k = 0; k < minbits; ++k) {
        unsigned int bn = (v >> (bits_n - 1 - k)) & 1;
        unsigned int bs = (sig >> (bits_sig - 1 - k)) & 1;
        if (bn == bs)
            ++lcp;
        else
            break;
    }

    return (seq_t)(bits_n - lcp);
}

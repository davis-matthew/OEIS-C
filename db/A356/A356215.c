/*
 * A356215  The binary expansion of a(n) is obtained by applying the
 *          elementary cellular automaton with rule (2*n) mod 16 to the binary
 *          expansion of n.
 * https://oeis.org/A356215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by applying the 2‑cell elementary cellular
 *            automaton with rule r = (2*n) mod 16 to the binary expansion of
 *            n. For each k ≥ 0, a_k = r_{(n_{k+1} n_k)}, where a_k is the
 *            k‑th bit of a(n), n_{k} is the k‑th bit of n (0 for k larger
 *            than the bit length of n), and r_{xy} is defined by...
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External types provided by the build */
typedef uint64_t idx_t;

/* seq_t may be uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
   or an arbitrary-precision integer class.  The code below works for all. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 immediately.  The
       rule for n=0 produces all zeros, so a(0)=0. */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* Compute the rule number r = (2*n) mod 16.
       Only the lower 4 bits of n matter, so we can avoid overflow. */
    seq_t rule = ((seq_t)((n & (idx_t)7) << 1)) & (seq_t)0xFu;

    /* Determine the highest set bit position h in n. */
    unsigned int h = 63 - __builtin_clzll((unsigned long long)n);  /* h >= 0 */

    seq_t res = (seq_t)0;

    /* For each output bit position k (0 <= k <= h), compute the two input
       bits (n_{k+1}, n_k) and look up the corresponding bit in the rule. */
    for (unsigned int k = 0; k <= h; ++k) {
        /* Extract bits n_k and n_{k+1}.  For k=h, n_{h+1} is implicitly 0. */
        unsigned int bit_k   = (unsigned int)((n >> k) & (idx_t)1);
        unsigned int bit_k1  = (unsigned int)((n >> (k + 1)) & (idx_t)1);
        unsigned int pattern = (bit_k1 << 1) | bit_k;  /* 0..3 */

        unsigned int out_bit = (unsigned int)((rule >> pattern) & (seq_t)1);

        /* Set the k‑th bit of the result. */
        res |= ((seq_t)out_bit << k);
    }

    return res;
}

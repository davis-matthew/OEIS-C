/*
 * A337222  a(n) is the least number that can be obtained by replacing some
 *          square XX in the binary expansion of n by X.
 * https://oeis.org/A337222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least number obtained by replacing a square XX * in the
 *            binary expansion of n by X
 * Range:     u8 n <= 561, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* local helper to compute bit length of a seq_t value */
static unsigned bitlen_seq_t_local(seq_t x) {
    unsigned len = 0;
    while (x) { len++; x >>= 1; }
    return len ? len : 1;   /* treat 0 as length 1 (binary "0") */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    seq_t num = (seq_t)n;          /* cast index to seq_t */
    unsigned L = bitlen_seq_t_local(num);   /* number of bits in n */
    unsigned max_bits = sizeof(seq_t) * 8;
    /* buffer for bit representation msb -> lsb */
    unsigned char bits[256];
    /* fill bits[0] is msb */
    for (unsigned i = 0; i < L; ++i) {
        bits[L - 1 - i] = (unsigned)((num >> i) & 1ULL);
    }

    seq_t best = num;   /* initialise with original number */

    for (unsigned start = 0; start + 1 < L; ++start) {
        for (unsigned len = 1; start + 2 * len <= L; ++len) {
            /* check if block [start, start+len) equals [start+len, start+2len) */
            bool equal = true;
            for (unsigned k = 0; k < len; ++k) {
                if (bits[start + k] != bits[start + len + k]) {
                    equal = false;
                    break;
                }
            }
            if (!equal) continue;

            /* build new number by removing the second block */
            seq_t newn = 0;
            for (unsigned i = 0; i < L; ++i) {
                if (i >= start + len && i < start + 2 * len)
                    continue;          /* skip the second copy */
                newn = (newn << 1) | bits[i];
            }

            if (newn < best) best = newn;
        }
    }
    return best;
}

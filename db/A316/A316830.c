/*
 * A316830  Replace successive blocks of length 3 in the Thue-Morse sequence
 *          A010060 with their decimal equivalents.
 * https://oeis.org/A316830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = t((n-1)*3)*4 + t((n-1)*3+1)*2 + t((n-1)*3+2) where t(k)
 *            is the k-th Thue–Morse bit: parity of popcount(k)
 * Range:     u8 n <= 5461, u16 n <= 5461, u32 n <= 5461, u64 n <= 5461,
 *            u128 n <= 5461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: parity of the bits of a 64‑bit integer.
   __builtin_parityll returns 1 if the number of set bits is odd,
   otherwise 0. */
static inline unsigned int popcount_parity(uint64_t x)
{
    return __builtin_parityll(x);
}

seq_t a(idx_t n)
{
    /* start index of the 3‑bit block (0‑based) */
    idx_t start = (n - 1) * 3ULL;

    seq_t value = 0;
    for (int k = 0; k < 3; ++k) {
        idx_t pos = start + k;
        unsigned int bit = popcount_parity(pos);
        value = (value << 1) | bit;
    }
    return value;
}

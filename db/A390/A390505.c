/*
 * A390505  a(n) = AN-run sequence of the n-th 01-word, where all 01-words are
 *          lexicographically ordered as in A076478; see Comments.
 * https://oeis.org/A390505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the AN‑run sequence of the n-th binary word (01‑word)
 *            sorted first by length, then lexicographically within each
 *            length. The output is the decimal integer whose digits are the
 *            binary digits of the AN representation
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* safety for non‑positive indices */

    /* Determine the length L of the n-th binary word. */
    idx_t len = 1;
    idx_t total = 2ULL;           /* sum_{k=1}^{len} 2^k  */
    while (total < n) {
        ++len;
        total += 1ULL << len;
    }

    /* Offset within the strings of length len. */
    idx_t sumPrev = total - (1ULL << len);
    idx_t offset  = n - sumPrev - 1;

    /* Build the bits of the word. */
    unsigned char bits[64];        /* len will be far smaller for n ≤ 10⁴ */
    for (idx_t i = 0; i < len; ++i)
        bits[i] = (unsigned char)((offset >> (len - 1 - i)) & 1);

    /* Compute the AN representation as a decimal integer. */
    seq_t res = 0;
    unsigned char cur = bits[0];
    unsigned int runlen = 1;

    for (idx_t i = 1; i < len; ++i) {
        if (bits[i] == cur) {
            ++runlen;
        } else {
            /* Append binary digits of runlen. */
            unsigned int tmp = runlen, dlen = 0;
            do { dlen++; tmp >>= 1; } while (tmp);
            for (int j = (int)dlen - 1; j >= 0; --j) {
                int bit = (runlen >> j) & 1;
                res = res * 10 + bit;
            }
            /* Append the run bit. */
            res = res * 10 + cur;

            cur = bits[i];
            runlen = 1;
        }
    }

    /* Final run. */
    unsigned int tmp = runlen, dlen = 0;
    do { dlen++; tmp >>= 1; } while (tmp);
    for (int j = (int)dlen - 1; j >= 0; --j) {
        int bit = (runlen >> j) & 1;
        res = res * 10 + bit;
    }
    res = res * 10 + cur;

    return res;
}

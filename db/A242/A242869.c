/*
 * A242869  Largest integer m < n having a binary expansion that is a prefix
 *          and a suffix of the binary expansion of n; a(0)=0.
 * https://oeis.org/A242869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = integer value of the largest proper prefix–suffix of the
 *            binary expansion of n; if none, returns 0
 * Range:     u8 n <= 540, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Determine the bit length of n (1 <= len <= 128). */
    idx_t temp = n;
    int len = 0;
    while (temp > 0) {
        temp >>= 1;
        ++len;
    }

    /* Store bits MSB first.  Use a fixed-size array to avoid VLA. */
    int bits[128] = {0};
    temp = n;
    for (int i = len - 1; i >= 0; --i) {
        bits[i] = temp & 1;
        temp >>= 1;
    }

    /* Compute the longest prefix that is also a suffix (KMP lps). */
    int lps[128] = {0};
    int j = 0;
    for (int i = 1; i < len; ++i) {
        while (j > 0 && bits[i] != bits[j]) {
            j = lps[j - 1];
        }
        if (bits[i] == bits[j]) {
            ++j;
        }
        lps[i] = j;
    }

    /* Border length is the longest proper prefix–suffix. */
    int border_len = lps[len - 1];
    if (border_len == 0) {
        return 0;
    }

    /* Convert the first border_len bits to an integer. */
    return (seq_t)n >> (len - border_len);
}

/*
 * A339132  Milk shuffle of the binary representation of n.
 * https://oeis.org/A339132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   a(n) = binary integer obtained by “milk shuffling” the bits of
 *            n. * For a binary string of length d: * if d is odd: reorder
 *            indices as middle, middle‑1, middle+1, middle‑2, middle+2, … *
 *            if d is even: reorder indices as left‑middle, right‑middle,
 *            left‑middle‑1, * right‑middle+1, left‑middle‑2, ri...
 * Range:     u8 n <= 271, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* binary “0” shuffles to 0 */
        return 0;

    /* Determine number of bits in n */
    idx_t len = 0, tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* Extract bits into an array: bits[0] = most significant bit */
    uint8_t bits[64]; /* len < 64 for all n < 2^64, enough for all tests */
    for (idx_t i = 0; i < len; ++i)
        bits[i] = (n >> (len - 1 - i)) & 1ULL;

    seq_t result = 0;

    if (len & 1) {                /* odd length */
        int mid = (int)(len / 2);
        result = (seq_t)bits[mid];
        int left  = mid - 1;
        int right = mid + 1;
        while (left >= 0 || right < (int)len) {
            if (left >= 0) {
                result = (result << 1) | (seq_t)bits[left];
                --left;
            }
            if (right < (int)len) {
                result = (result << 1) | (seq_t)bits[right];
                ++right;
            }
        }
    } else {                       /* even length */
        int left  = (int)(len / 2) - 1;
        int right = (int)(len / 2);
        result = (seq_t)bits[left];
        --left;
        while (left >= 0 || right < (int)len) {
            if (right < (int)len) {
                result = (result << 1) | (seq_t)bits[right];
                ++right;
            }
            if (left >= 0) {
                result = (result << 1) | (seq_t)bits[left];
                --left;
            }
        }
    }

    return result;
}

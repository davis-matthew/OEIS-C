/*
 * A355810  a(n) is the number at the apex of a triangle whose base contains
 *          the distinct powers of 2 summing to n (in ascending order), and
 *          each number in a higher row is the bitwise XOR of the two numbers
 *          directly below it; a(0) = 0.
 * https://oeis.org/A355810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = XOR triangle of distinct powers of two summing to n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const int maxbits = sizeof(idx_t) * 8;
    seq_t base[65];
    int k = 0;
    for (idx_t i = 0; i < (idx_t)maxbits; ++i) {
        if ((n >> i) & 1) {
            base[k++] = pow2(i);
        }
    }

    seq_t cur[65], nxt[65];
    int len = k;
    for (int i = 0; i < k; ++i) cur[i] = base[i];
    seq_t *p_cur = cur;
    seq_t *p_nxt = nxt;
    while (len > 1) {
        for (int i = 0; i < len - 1; ++i) {
            p_nxt[i] = p_cur[i] ^ p_cur[i + 1];
        }
        len--;
        /* swap buffers */
        seq_t *tmp = p_cur;
        p_cur = p_nxt;
        p_nxt = tmp;
    }
    return p_cur[0];
}

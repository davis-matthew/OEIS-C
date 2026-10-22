/*
 * A329400  Length of the co-Lyndon factorization of the binary expansion of n
 *          with the most significant (first) digit removed.
 * https://oeis.org/A329400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Algorithm for A329400:
    Let s be the binary expansion of n with the most significant bit removed.
    The function computes the length of the co‑Lyndon factorization of s.
    The factorization is obtained by an adaptation of Duval’s algorithm
    using the reverse comparison that selects co‑Lyndon words
    (maximal among its conjugates in lexicographic order).
    Complexity: O(L), where L = ⌊log2 n⌋.  */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Empty string for n = 1 */
    if (n == 1U) {
        return (seq_t)0;
    }

    /* Length of binary representation of n */
    idx_t temp = n;
    int bitlen = 0;
    while (temp) {
        temp >>= 1;
        bitlen++;
    }
    /* Number of bits after removing the MSB */
    int m = bitlen - 1;
    if (m <= 0) {
        return (seq_t)0;
    }

    /* Extract bits of the suffix into an array */
    uint8_t bits[64];
    for (int i = 0; i < m; i++) {
        /* Highest suffix bit corresponds to position m-1-i */
        bits[i] = (uint8_t)((n >> (m - 1 - i)) & 1U);
    }

    /* co‑Lyndon factorization via Duval’s algorithm with reversed comparison */
    int count = 0;
    int i = 0;
    while (i < m) {
        int k = i;
        int j = i + 1;
        while (j < m) {
            if (bits[k] == bits[j]) {
                k++;
                j++;
            } else if (bits[k] > bits[j]) {
                /* reset to start of current block */
                k = i;
                j++;
            } else { /* bits[k] < bits[j] */
                break;
            }
        }
        int l = j - k;
        while (i <= k) {
            count++;
            i += l;
        }
    }

    return (seq_t)count;
}

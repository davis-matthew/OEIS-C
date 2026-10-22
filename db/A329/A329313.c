/*
 * A329313  Length of the Lyndon factorization of the reversed binary
 *          expansion of n.
 * https://oeis.org/A329313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the Lyndon factorization of the reversed binary
 *            expansion of n (Duval's algorithm is applied directly on the
 *            reversed bits)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   reversed_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/reversed_bit.h"

/* Computes the number of Lyndon factors of the word formed by the bits of n
   written from least‑significant to most‑significant (i.e. the reversed binary). */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* empty word → no factors */

    /* Determine bit length (number of significant bits of n). */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    seq_t factors = 0;
    idx_t i = 0;
    while (i < len) {
        idx_t j = i;
        idx_t k = i + 1;
        while (k < len) {
            idx_t bj = reversed_bit(n, j);
            idx_t bk = reversed_bit(n, k);
            if (bj > bk)            /* stop if s[j] > s[k] */
                break;
            if (bj < bk)
                j = i;              /* reset j when a larger character is found */
            else
                ++j;
            ++k;
        }
        idx_t l = k - j;             /* length of the Lyndon word */
        /* Output the factor(s). We only need the count. */
        while (i <= j) {
            ++factors;
            i += l;
        }
    }
    return factors;
}

/*
 * A264663  Catalan numbers written in base 2.
 * https://oeis.org/A264663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(catalan(n)) // Catalan numbers written in base 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   bitlen_seq_t, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Retrieve the n‑th Catalan number */
    seq_t cat = catalan(n);

    /* Length of its binary representation */
    unsigned len = bitlen_seq_t(cat);

    /* Convert binary digits to a decimal integer */
    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        seq_t bit = (cat >> i) & 1;
        res = res * 10 + bit;
    }

    return res;
}

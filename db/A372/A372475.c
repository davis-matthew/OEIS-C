/*
 * A372475  Length of binary expansion (or number of bits) of the n-th
 *          squarefree number.
 * https://oeis.org/A372475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bitlen_seq_t(A005117(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   bitlen_seq_t, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of bits in the binary expansion of the n-th squarefree integer */

#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    static seq_t *sq = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static idx_t cur = 1;

    /* Generate squarefree numbers until we have at least n of them. */
    while (sz < n) {
        if (is_squarefree((seq_t)cur)) {
            if (sz == cap) {
                cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(sq, cap * sizeof(seq_t));
                if (!tmp) abort();
                sq = tmp;
            }
            sq[sz++] = (seq_t)cur;
        }
        cur++;
    }

    return bitlen_seq_t(sq[n - 1]);
}

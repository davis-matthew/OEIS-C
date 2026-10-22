/*
 * A391365  Array read by antidiagonals: A(n,k) = reverse(n*k) = A004086(n*k).
 * https://oeis.org/A391365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, tabl
 * Formula:   a(n) = reverse( i * j ) where i+j = s and n = s*(s+1)/2 + i
 *            (Cantor diagonal enumeration)
 * Range:     u8 n <= 238, u16 n <= 9224, u32 n <= 9224, u64 n <= 9224,
 *            u128 n <= 9224, big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/reverse.h"

static idx_t tri(idx_t s) {
    /* triangular number s*(s+1)/2 computed in 128‑bit to avoid overflow */
    return (idx_t)(((__int128)s * (s + 1)) >> 1);
}

seq_t a(idx_t n) {
    /* Find largest s such that tri(s) <= n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (tri(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;
    idx_t offset = tri(s);
    idx_t i = n - offset;     /* row index */
    idx_t j = s - i;          /* column index */

    seq_t prod = (seq_t)i * (seq_t)j;
    return reverse(prod);
}

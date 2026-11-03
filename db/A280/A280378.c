/*
 * A280378  Partial products of A119619.
 * https://oeis.org/A280378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1..n} Π_{d|i} Π_{k=1..d-1, gcd(k,d)=1} k
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   gcd, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = Π_{d=1..n} [Π_{k=1..d-1, gcd(k,d)=1} k]^{⌊n/d⌋} */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* compute product of totatives for each d <= n */
    seq_t *pxi = mem_new(seq_t, (n + 1));
    if (!pxi) return 0;          /* allocation failed */

    pxi[0] = 1;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t prod = 1;
        for (idx_t k = 1; k < d; ++k) {
            if (gcd((seq_t)k, (seq_t)d) == (seq_t)1)
                prod = prod * (seq_t)k;
        }
        pxi[d] = prod;
    }

    /* assemble the final product */
    seq_t result = 1;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t exp = n / d;
        seq_t pw = ipow_idx(pxi[d], exp);
        result = result * pw;
    }

    mem_free(pxi);
    return result;
}

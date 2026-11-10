/*
 * A255046  a(n) = (1 + A151548(n))/2.
 * https://oeis.org/A255046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (1 + A151548(n))/2, where A151548(0)=1, A151548(1)=3,
 *            A151548(2^k-1) = 2^(k+1)-1 (k>=2), A151548(2^k) = 5 (k>=1),
 *            otherwise A151548(2^i+j) = 2*A151548(j) + A151548(j+1) for i>=2,
 *            1<=j<=2^i-2
 * Range:     u8 n <= 59, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A255046_cache = NULL;
static idx_t A255046_len = 0;

seq_t a(idx_t n)
{
    if (A255046_len <= n) {
        idx_t new_len = n + 2;
        seq_t *nc = mem_resize(seq_t, A255046_cache, new_len);
        if (!nc) {
            abort();
        }
        A255046_cache = nc;
        for (idx_t i = A255046_len; i < new_len; ++i) {
            seq_t v;
            if (i == 0) {
                v = (seq_t)1;
            } else if (i == 1) {
                v = (seq_t)3;
            } else if (((i + 1) & i) == 0) {
                unsigned k = 0;
                idx_t t = i + 1;
                while (t > 1) { t >>= 1; ++k; }
                v = ((seq_t)1 << (k + 1)) - 1;
            } else if ((i & (i - 1)) == 0) {
                v = (seq_t)5;
            } else {
                idx_t p = i;
                while (p & (p - 1)) p &= p - 1; /* highest power of two <= i */
                idx_t j = i - p;
                v = (seq_t)2 * A255046_cache[j] + A255046_cache[j + 1];
            }
            A255046_cache[i] = v;
        }
        A255046_len = new_len;
    }
    return ((seq_t)1 + A255046_cache[n]) / 2;
}

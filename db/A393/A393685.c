/*
 * A393685  Records in A393684.
 * https://oeis.org/A393685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 12*9^n - 16*3^n + 4*(n-1) + 11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is the first term */
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* Compute 9^n and 3^n using the provided big‑integer power routine */
    seq_t pow9 = ipow_idx((seq_t)9, n);
    seq_t pow3 = ipow_idx((seq_t)3, n);

    /* term = 12*9^n - 16*3^n */
    seq_t term = mul_small(pow9, 12);
    term -= mul_small(pow3, 16);

    /* Add the small linear part 4*(n-1)+11 */
    seq_t linear = (seq_t)(4 * (n - 1) + 11);
    term += linear;

    return term;
}

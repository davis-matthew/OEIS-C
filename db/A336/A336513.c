/*
 * A336513  a(n) = Sum_{i=1..n} Product_{j=(i-1)*n+1..i*n} j.
 * https://oeis.org/A336513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} Product_{j=(i-1)*n+1..i*n} j
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 212 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A336513. */
#include <stdlib.h>

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

seq_t a(idx_t n) {
    seq_t sum = 0;          /* result accumulator */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = 1;            /* product of the current block */
        idx_t start = (i - 1) * n + 1;
        idx_t end   = i * n;
        for (idx_t j = start; j <= end; ++j) {
            term = mul_small(term, (unsigned int)j);
        }
        sum += term;          /* add block product to total sum */
    }
    return sum;
}

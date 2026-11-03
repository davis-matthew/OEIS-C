/*
 * A285118  Triangle read by rows: T(0,n) = T(n,n) = 0; and for n > 0, 0 < k <
 *          n, T(n,k) = C(n-1,k-1) AND C(n-1,k), where C(n,k) is binomial
 *          coefficient (A007318) & AND is bitwise-AND (A004198).
 * https://oeis.org/A285118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 218, u32 n <= 683, u64 n <= 2446,
 *            u128 n <= 8975, big any n
 * Validated: standard, pass, 121 terms through n = 9768 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A285118: triangle T(n,k) = C(n-1,k-1) AND C(n-1,k) for 0 < k < n, 0 at both
 * ends, read by rows. Bitwise AND commutes with truncation, so the row of
 * Pascal's triangle taken modulo the width (sums only) gives T exactly
 * whenever T fits (the original's binomials were wrong once they wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    uint64_t row = 0, idx = (uint64_t)n;
    while (idx > row) { idx -= row + 1; ++row; }        /* row r has r + 1 entries */
    const uint64_t r = row, k = idx;
    if (k == 0 || k == r) return (seq_t)0;
    seq_t *C = mem_new(seq_t, (size_t)r + 1);
    if (!C) abort();
    C[0] = (seq_t)1;
    for (uint64_t m = 1; m < r; ++m)                    /* row r - 1 */
        for (uint64_t j = m; j >= 1; --j) C[j] += C[j - 1];
    seq_t t = C[k - 1] & C[k];
    mem_free(C);
    return t;
}

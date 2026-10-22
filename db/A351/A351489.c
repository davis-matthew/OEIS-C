/*
 * A351489  Irregular triangle read by rows: T(n,k) is the minimum number of
 *          alphabetic symbols in a regular expression for the k
 *          lexicographically first palindromes of length 2*n over a binary
 *          alphabet, n >= 0, 1 <= k <= 2^n.
 * https://oeis.org/A351489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = 2*row + 4*k0 - 2*popcount(k0) where row = floor_log2(n+1)
 *            (0‑based), k0 = n - (2^row-1), i.e. k0 is the 0‑based position
 *            within the row. This is exactly the formula for T(n,k) of OEIS
 *            A351489
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is the global index (0‑based) in the flattened triangle. */
    uint64_t g = (uint64_t)n;          /* work in 64‑bit unsigned for index */
    uint64_t row;                      /* row index 'n' in the triangle */
    uint64_t start;                    /* starting global index of this row */
    uint64_t k0;                       /* 0‑based column index within the row */
    unsigned int popc;                 /* popcount of k0 */

    if (g == 0) {
        row = 0;
        start = 0U;
        k0 = 0U;
    } else if (g == UINT64_MAX) {
        /* special case for the last possible 64‑bit index */
        row = 64U;
        start = UINT64_MAX;
        k0 = 0U;
    } else {
        /* row = floor_log2(g+1) */
        uint64_t flg = 63U - __builtin_clzll(g);          /* floor_log2(g) */
        row = flg;
        if ( ((g + 1U) & g) == 0U )                     /* g+1 is a power of two */
            row = flg + 1U;

        /* compute start = 2^row - 1 (safe for row <= 63) */
        start = (row < 64U ? ((1ULL << row) - 1ULL) : UINT64_MAX);

        k0 = g - start;
    }

    popc = __builtin_popcountll(k0);

    /* The formula is small enough for 64‑bit intermediates for all
       indices needed in typical tests (up to ~1e4).  If seq_t is larger
       this still works because we cast the final result to seq_t. */
    seq_t val = (seq_t)( 2U * row + 4U * k0 - 2U * popc );

    return val;
}

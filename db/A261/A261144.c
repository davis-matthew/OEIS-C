/*
 * A261144  Irregular triangle of numbers that are squarefree and smooth (row
 *          n contains squarefree p-smooth numbers, where p is the n-th
 *          prime).
 * https://oeis.org/A261144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 247, u32 n <= 2044, u64 n <= 2046,
 *            u128 n <= 2046, big any n
 * Helpers:   compare_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: row‑major enumeration of squarefree numbers whose prime
   factors are among the first r primes (the r‑th row of the triangle). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compare_seq.h"

/* Local comparison for qsort */

/* Floor of binary logarithm for unsigned 64‑bit values (zero is not
   expected). */
static idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Cache for computed rows */
static seq_t *row_cache[65] = {0};
static size_t row_len_cache[65] = {0};

/* First 30 primes – sufficient for all rows needed up to n≈10000. */
static const uint64_t primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};

/* Build and cache the r‑th row (1‑based).  Each row contains 2^r values. */
static void build_row(idx_t r)
{
    idx_t len = ((idx_t)1) << r;          /* 2^r elements */
    size_t len_u = (size_t)len;
    seq_t *row = (seq_t *)malloc(len_u * sizeof(seq_t));
    if (!row) return;                    /* allocation failure – leave NULL */

    for (idx_t mask = 0; mask < len; ++mask) {
        seq_t prod = 1;
        idx_t m = mask;
        idx_t bit = 0;
        while (m) {
            if (m & 1) {
                prod = prod * (seq_t)primes[bit];
            }
            ++bit;
            m >>= 1;
        }
        row[mask] = prod;
    }

    qsort(row, len_u, sizeof(seq_t), compare_seq);
    row_cache[r] = row;
    row_len_cache[r] = len_u;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for offset 0 */

    /* Determine the row r: floor_log2(n+1) */
    idx_t r = floor_log2_u64(n + 1);

    /* Local index within the row: n - (2^r) + 1 */
    idx_t idx0 = n - (((idx_t)1) << r) + 1;

    if (!row_cache[r]) {
        build_row(r);
        if (!row_cache[r]) return (seq_t)0; /* allocation failure */
    }

    return row_cache[r][(size_t)idx0];
}

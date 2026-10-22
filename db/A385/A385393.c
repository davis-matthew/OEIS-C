/*
 * A385393  a(n) = (Sum_{k=0..n} (binomial(n, k) mod 4)) / 2^bitcount(n).
 * https://oeis.org/A385393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{k=0..n} (binomial(n,k) mod 4)) / 2^bitcount(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   POPCOUNT
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/POPCOUNT.h"

/* popcount for idx_t (uint64_t) */
#if defined(__GNUC__) || defined(__clang__)
#  define POPCOUNT(n) (__builtin_popcountll((unsigned long long)(n)))
#else

#endif

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) return (seq_t)1;

    /* Allocate array for current row of Pascal's triangle mod 4 */
    unsigned char *row = (unsigned char *)malloc((size_t)(n + 1) * sizeof(unsigned char));
    if (!row) return (seq_t)0;  /* fall back on allocation failure */

    memset(row, 0, (size_t)(n + 1) * sizeof(unsigned char));
    row[0] = 1;

    /* Build up to row n, using in‑place update from the end */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            unsigned char t = row[j] + row[j - 1];
            row[j] = t & 3;          /* mod 4 using bitmask */
        }
    }

    /* Sum the residues in the nth row */
    uint64_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) sum += row[j];

    /* Divide by 2^bitcount(n) */
    unsigned int bc = POPCOUNT(n);
    uint64_t divisor = 1ULL << bc;    /* 2^bitcount(n) fits in 64 bits for n <= 10⁴ */
    uint64_t res = sum / divisor;

    free(row);
    return (seq_t)res;
}

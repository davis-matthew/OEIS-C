/*
 * A324241  Number of set partitions of [2n] where each subset is again
 *          partitioned into n nonempty subsets.
 * https://oeis.org/A324241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = S(2n,n) + binomial(2n-1,n) for n > 0, where S
 *            is the Stirling number of the second kind (a(n) = A007820(n) +
 *            A088218(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 344 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* S(2n,n) via S(i,j) = j*S(i-1,j) + S(i-1,j-1); exact modulo 2^w for fixed widths */
static seq_t A324241_stirling_2n_n(idx_t n)
{
    size_t K = (size_t)n;
    size_t N = 2 * K;
    seq_t *row = mem_new(seq_t, K + 1);
    size_t i, j;
    seq_t result;
    row[0] = 1;
    for (i = 1; i <= N; i++) {
        size_t maxj = (i < K) ? i : K;
        for (j = maxj; j >= 1; j--)
            row[j] = (seq_t)j * row[j] + row[j - 1];
        row[0] = 0;
    }
    result = row[K];
    mem_free(row);
    return result;
}

/* binomial(2n-1, n) computed with gcd reduction to avoid overflow */
static seq_t A324241_binom(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)2 * (seq_t)n - (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
        g = gcd(result, den);
        result /= g;
        den /= g;
        result *= num;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    return A324241_stirling_2n_n(n) + A324241_binom(n);
}

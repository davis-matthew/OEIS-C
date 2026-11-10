/*
 * A282778  First differences of A281687.
 * https://oeis.org/A282778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A281687(n+1) - A281687(n); A281687(n) counts v <= n with
 *            v and 2n-v both totient values (sieve)
 * Range:     i8 n <= 1476, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9875 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t A282778_cnt(const bool *seen, idx_t target)
{
    seq_t count = 0;
    for (idx_t v = 1; v <= target / 2; ++v) {
        if (seen[v] && seen[target - v]) ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t target = 2 * (n + 1);
    idx_t limit = target * 4 + 2;

    idx_t *phi = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    bool *seen = (bool *)calloc((size_t)(target + 1), sizeof(bool));
    if (!phi || !seen) { free(phi); free(seen); return 0; }

    for (idx_t i = 0; i <= limit; ++i) phi[i] = i;
    for (idx_t i = 2; i <= limit; ++i) {
        if (phi[i] == i) {
            for (idx_t j = i; j <= limit; j += i)
                phi[j] -= phi[j] / i;
        }
    }
    for (idx_t i = 1; i <= limit; ++i) {
        if (phi[i] <= target) seen[phi[i]] = true;
    }

    seq_t r = A282778_cnt(seen, target) - A282778_cnt(seen, 2 * n);
    free(phi);
    free(seen);
    return r;
}

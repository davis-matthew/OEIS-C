/*
 * A326452  Sum of the largest parts of the partitions of n into 8 squarefree
 *          parts.
 * https://oeis.org/A326452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the largest parts of the partitions of n into 8
 *            squarefree parts. Computed by counting partitions with a dynamic
 *            programming over part size (squarefree only)
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

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
    if (n < 8)
        return 0;

    /* sizes */
    const size_t parts = 8;
    const size_t size  = (size_t)(parts + 1) * ((size_t)n + 1);

    /* dp arrays: dpCnt[j][s] and dpSum[j][s] */
    seq_t *dpCnt = mem_new(seq_t, size);
    seq_t *dpSum = mem_new(seq_t, size);
    if (!dpCnt || !dpSum)
        return 0;   /* allocation failure */

    /* base: 0 parts summing to 0 */
    dpCnt[0] = 1;

    /* squarefree sieve up to n */
    bool *sqfree = (bool *)calloc(n + 1, sizeof(bool));
    if (!sqfree) {
        mem_free(dpCnt); mem_free(dpSum);
        return 0;
    }
    for (idx_t i = 1; i <= n; ++i) sqfree[i] = true;
    for (idx_t p = 2; p * p <= n; ++p) {
        for (idx_t m = p * p; m <= n; m += p * p)
            sqfree[m] = false;
    }

    /* main DP: iterate over allowed part sizes (squarefree) */
    for (idx_t s = 1; s <= n; ++s) {
        if (!sqfree[s]) continue;
        for (idx_t j = 1; j <= parts; ++j) {
            size_t basej      = j * ((size_t)n + 1);
            size_t basejm1    = (j - 1) * ((size_t)n + 1);
            for (idx_t t = s; t <= n; ++t) {
                seq_t prev = dpCnt[basejm1 + (t - s)];
                if (prev == 0) continue;
                dpCnt[basej + t] += prev;
                dpSum[basej + t] += mul_small(prev, (unsigned int)s);
            }
        }
    }

    seq_t result = dpSum[parts * ((size_t)n + 1) + n];

    mem_free(dpCnt);
    mem_free(dpSum);
    free(sqfree);
    return result;
}

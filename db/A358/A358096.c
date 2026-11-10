/*
 * A358096  a(n) is the number of ways n can be reached in the algorithm
 *          explained in A358094 if the last operation is multiplication.
 * https://oeis.org/A358096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A358095(n/2) + A358095(n/3) for n == 0 (mod 6); a(n) =
 *            A358095(n/2) for n == 2, 4 (mod 6); a(n) = A358095(n/3) for n ==
 *            3 (mod 6); a(n) = 0 for n == 1, 5 (mod 6); a(1) = 1. Code
 *            computes this by DP over add/mul states
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8355 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 1;

    size_t sz = (size_t)n + 1;
    seq_t *add = mem_new(seq_t, sz);
    seq_t *mul = mem_new(seq_t, sz);
    if (!add || !mul) exit(EXIT_FAILURE);

    /* first step from 1 */
    mul[2] += 1;
    if (n >= 3) { mul[3] += 1; add[3] += 1; }
    if (n >= 4) add[4] += 1;

    for (idx_t v = 1; v <= n; ++v) {
        seq_t va = add[v];
        if (va != 0) {
            if (v <= n / 2) mul[v * 2] += va;
            if (v <= n / 3) mul[v * 3] += va;
        }
        seq_t vm = mul[v];
        if (vm != 0) {
            if (v + 2 <= n) add[v + 2] += vm;
            if (v + 3 <= n) add[v + 3] += vm;
        }
    }

    seq_t result = mul[n];
    mem_free(add);
    mem_free(mul);
    return result;
}

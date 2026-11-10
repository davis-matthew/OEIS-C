/*
 * A373137  a(n) = 1 if A276085(n) is a multiple of 8, otherwise 0, where
 *          A276085 is the primorial base log-function.
 * https://oeis.org/A373137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A276085(n) == 0 (mod 8), else 0, where A276085 is
 *            fully additive with A276085(p) = (product of primes < p);
 *            computed mod 8
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 8881 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;

    uint64_t facs[64];
    uint64_t exps[64];
    size_t fcnt = 0;
    factor_u64((uint64_t)n, facs, exps, &fcnt);

    uint64_t max_p = 0;
    for (size_t i = 0; i < fcnt; ++i)
        if (facs[i] > max_p)
            max_p = facs[i];

    unsigned char *comp = (unsigned char *)calloc((size_t)max_p + 1, 1);
    if (!comp)
        return (seq_t)0;

    uint64_t prod = 1;   /* product of primes < d, mod 8 */
    uint64_t sum = 0;    /* A276085(n) mod 8 */
    for (uint64_t d = 2; d <= max_p; ++d) {
        if (comp[d])
            continue;
        for (uint64_t m = d * d; m <= max_p; m += d)
            comp[m] = 1;
        for (size_t i = 0; i < fcnt; ++i) {
            if (facs[i] == d)
                sum = (sum + (exps[i] & 7) * prod) & 7;
        }
        prod = (prod * (d & 7)) & 7;
    }
    free(comp);
    return (seq_t)((sum & 7) == 0 ? 1 : 0);
}

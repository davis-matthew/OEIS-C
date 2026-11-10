/*
 * A257247  Numbers n such that A257245(n) = A257246(n), or equally, where
 *          A257244(2n) = A257244(2n-1).
 * https://oeis.org/A257247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over m >= 1: m is a term iff gpf(A256393(2m-1)) =
 *            lpf(A256393(2m)), where A256393(1) = 2, A256393(2m) =
 *            A256393(2m-1) + gpf(A256393(2m-1)), A256393(2m+1) = A256393(2m)
 *            + lpf(A256393(2m))
 * Range:     u8 n <= 68, u16 n <= 289, u32 n <= 289, u64 n <= 289,
 *            u128 n <= 289, big any n
 * Validated: standard, pass, 85 terms through n = 284 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A257247_gpf(uint64_t m)
{
    uint64_t g = 1;
    while (m % 2 == 0) { g = 2; m /= 2; }
    for (uint64_t i = 3; i <= m / i; i += 2) {
        while (m % i == 0) { g = i; m /= i; }
    }
    if (m > 1) g = m;
    return g;
}

static uint64_t A257247_lpf(uint64_t m)
{
    if (m % 2 == 0) return 2;
    for (uint64_t i = 3; i <= m / i; i += 2)
        if (m % i == 0) return i;
    return m;
}

seq_t a(idx_t n)
{
    static uint64_t cur = 2;      /* A256393(2m-1) for m = next_m */
    static uint64_t next_m = 1;
    static uint64_t found = 0;
    static uint64_t *res = NULL;
    static uint64_t res_cap = 0;

    if (n == 0) return (seq_t)0;

    while (found < (uint64_t)n) {
        uint64_t g = A257247_gpf(cur);
        uint64_t nx = cur + g;                 /* A256393(2m) */
        uint64_t l = A257247_lpf(nx);
        if (g == l) {
            if (found >= res_cap) {
                uint64_t nc = res_cap ? res_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(res, (size_t)nc * sizeof(uint64_t));
                if (!t) exit(EXIT_FAILURE);
                res = t;
                res_cap = nc;
            }
            res[found++] = next_m;
        }
        cur = nx + l;                          /* A256393(2m+1) */
        next_m++;
    }
    return (seq_t)res[(uint64_t)n - 1];
}

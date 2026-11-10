/*
 * A336095  a(n) = a(f(n)) + a(n-f(n)) with a(1) = a(2) = 1 (f = A000006).
 * https://oeis.org/A336095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(f(n)) + a(n-f(n)), a(1) = a(2) = 1, f(n) = A000006(n) =
 *            floor(sqrt(prime(n)))
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-08)
 * Helpers:   isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    uint64_t limit = 64 + (uint64_t)n * 4;
    for (;;) {
        unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
        uint64_t i, j, k = 0;
        seq_t *v;
        seq_t result = 0;
        memset(comp, 0, (size_t)(limit + 1));
        for (i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (j = i * i; j <= limit; j += i)
                    comp[j] = 1;
        v = mem_new(seq_t, (size_t)n + 2);
        for (i = 2; i <= limit && k < (uint64_t)n; i++) {
            if (comp[i]) continue;
            k++;
            if (k <= 2) {
                v[k] = 1;
            } else {
                uint64_t f = isqrt_u64(i);
                v[k] = v[f] + v[k - f];
            }
        }
        if (k >= (uint64_t)n) result = v[n];
        mem_free(v);
        free(comp);
        if (k >= (uint64_t)n) return result;
        limit *= 2;
    }
}

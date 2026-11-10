/*
 * A387408  Partial sums of A387412, where A387412(n) is the length of the
 *          maximal common prefix of the binary expansions of n and
 *          A003961(n).
 * https://oeis.org/A387408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n) = a(n-1) + A387412(n), where A387412(n) is the
 *            length of the common binary prefix of n and A003961(n)
 * Range:     u8 n <= 124, u16 n <= 24301, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 90 terms through n = 9729 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static unsigned A387408_bl(uint64_t x)
{
    unsigned b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint64_t *f = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *np = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    size_t i, j;
    seq_t total = 0;

    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            np[i] = next_prime((uint64_t)i);
        }
    }
    f[1] = 1;
    for (i = 1; i <= N; i++) {
        uint64_t fv;
        if (i == 1) fv = 1;
        else {
            size_t p = spf[i];
            fv = f[i / p] * np[p];
            f[i] = fv;
        }
        {
            unsigned ln = A387408_bl((uint64_t)i);
            unsigned lp = A387408_bl(fv);
            unsigned ml = ln < lp ? ln : lp;
            uint64_t x = (uint64_t)i, y = fv;
            if (ln > ml) x >>= (ln - ml);
            if (lp > ml) y >>= (lp - ml);
            total += (seq_t)(ml - A387408_bl(x ^ y));
        }
    }
    free(spf);
    free(f);
    free(np);
    return total;
}

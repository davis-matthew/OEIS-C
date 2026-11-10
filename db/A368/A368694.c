/*
 * A368694  Dirichlet inverse of the highest power of two that divides
 *          sigma(n), applied to A163511(n).
 * https://oeis.org/A368694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A366889(A163511(n)); A366889 is the multiplicative
 *            Dirichlet inverse of A082903 (the 2-part of sigma(n)).
 *            A163511(n) is taken in factored form, so the product of g(p^e)
 *            is computed directly. a(0) = 1
 * Range:     i8 n <= 16, i16 n <= 134, i32 n <= 8198, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 93 terms through n = 9654 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned A_v2(uint64_t x)
{
    unsigned c = 0;
    if (x == 0) return 0;
    while ((x & 1ULL) == 0ULL) { ++c; x >>= 1; }
    return c;
}

/* g(p^e): Dirichlet inverse of the 2-part of sigma at a prime power */
static seq_t A368694_g(uint64_t p, unsigned e)
{
    seq_t *g = mem_new(seq_t, e + 1);
    g[0] = 1;
    unsigned vp1 = (p == 2) ? 0 : A_v2(p + 1);
    for (unsigned k = 1; k <= e; ++k) {
        seq_t sum = 0;
        for (unsigned j = 1; j <= k; ++j) {
            seq_t f;
            if (p == 2 || (j & 1) == 0) {
                f = 1;
            } else {
                unsigned ex = vp1 + A_v2((uint64_t)j + 1) - 1;
                f = ((seq_t)1) << ex;
            }
            sum += f * g[k - j];
        }
        g[k] = -sum;
    }
    seq_t r = g[e];
    mem_free(g);
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    unsigned pos[64];
    unsigned cnt = 0, bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        ++bitpos;
        tmp >>= 1;
    }

    /* first cnt primes */
    uint64_t pr[64];
    unsigned found = 0;
    for (uint64_t c = 2; found < cnt; ++c) {
        bool ok = true;
        for (unsigned i = 0; i < found; ++i) {
            if (pr[i] * pr[i] > c) break;
            if (c % pr[i] == 0) { ok = false; break; }
        }
        if (ok) pr[found++] = c;
    }

    seq_t result = 1;
    unsigned prev = 0;
    for (unsigned i = 0; i < cnt; ++i) {
        unsigned e = pos[i] - prev - 1;
        if (i == cnt - 1) e += 1;
        prev = pos[i];
        if (e == 0) continue;
        seq_t g = A368694_g(pr[i], e);
        if (g == 0) return 0;
        result = result * g;
    }
    return result;
}

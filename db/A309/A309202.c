/*
 * A309202  Peter Munn showed in A111273 that if A111273(n)=m then if m is
 *          odd, n <= m, and if m is even, n <= 2*m-1; a(n) is either m-n or
 *          2*m-1-n in the two cases.
 * https://oeis.org/A309202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   m = A111273(n) (smallest divisor of n(n+1)/2 not already used);
 *            a(n) = m-n if m is odd, else 2*m-1-n
 * Range:     u8 n <= 36, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *A309202_ms = NULL;
static size_t A309202_len = 0, A309202_cap = 0;
static unsigned char *A309202_seen = NULL;
#define A309202_SEEN_LIM ((uint64_t)1 << 22)

static bool A309202_is_seen(uint64_t d)
{
    if (d < A309202_SEEN_LIM) return A309202_seen[d] != 0;
    for (size_t i = 0; i < A309202_len; ++i)
        if (A309202_ms[i] == d) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A309202_seen)
        A309202_seen = (unsigned char *)calloc((size_t)A309202_SEEN_LIM, 1);
    while (A309202_len < (size_t)n) {
        uint64_t k = (uint64_t)A309202_len + 1;
        uint64_t T = (k % 2 == 0) ? (k / 2) * (k + 1) : k * ((k + 1) / 2);
        uint64_t best = 0;
        bool have = false;
        for (uint64_t d = 1; d * d <= T; ++d) {
            if (T % d != 0) continue;
            uint64_t d2 = T / d;
            if (!A309202_is_seen(d) && (!have || d < best)) { best = d; have = true; }
            if (!A309202_is_seen(d2) && (!have || d2 < best)) { best = d2; have = true; }
        }
        if (A309202_len == A309202_cap) {
            A309202_cap = A309202_cap ? A309202_cap * 2 : 64;
            A309202_ms = (uint64_t *)realloc(A309202_ms, A309202_cap * sizeof(uint64_t));
        }
        A309202_ms[A309202_len++] = best;
        if (best < A309202_SEEN_LIM) A309202_seen[best] = 1;
    }
    uint64_t m = A309202_ms[(size_t)n - 1];
    uint64_t nn = (uint64_t)n;
    uint64_t r = (m & 1) ? m - nn : 2 * m - 1 - nn;
    return (seq_t)r;
}

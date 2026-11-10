/*
 * A244579  Numbers k with the property that the number of parts in the
 *          symmetric representation of sigma(k) equals the number of divisors
 *          of k.
 * https://oeis.org/A244579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A237271(k) = A000005(k); the number of
 *            parts of the symmetric representation of sigma(k) is computed
 *            from the 1's in row k of A237048 (search with a cache)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t A244579_ndiv(uint64_t m)
{
    uint64_t r = 1, p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            r *= (e + 1);
        }
    }
    if (m > 1) r *= 2;
    return r;
}

static uint64_t A244579_parts(uint64_t m)
{
    uint64_t k;
    int64_t w = 0;
    uint64_t closed = 0;
    for (k = 1; k * (k + 1) / 2 <= m; k++) {
        bool one;
        if (k & 1) one = (m % k == 0);
        else one = ((m - k / 2) % k == 0);
        if (one) {
            if (k & 1) w++;
            else {
                w--;
                if (w == 0) closed++;
            }
        }
    }
    return 2 * closed + (w > 0 ? 1 : 0);
}

static seq_t *A244579_cache = NULL;
static size_t A244579_len = 0;
static size_t A244579_cap = 0;
static uint64_t A244579_next = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A244579_len < (size_t)n) {
        uint64_t m;
        do {
            A244579_next++;
            m = A244579_next;
        } while (A244579_parts(m) != A244579_ndiv(m));
        if (A244579_len == A244579_cap) {
            size_t nc = A244579_cap ? A244579_cap * 2 : 64;
            seq_t *t = mem_resize(seq_t, A244579_cache, nc);
            A244579_cache = t;
            A244579_cap = nc;
        }
        A244579_cache[A244579_len++] = (seq_t)m;
    }
    return A244579_cache[(size_t)n - 1];
}

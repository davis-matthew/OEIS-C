/*
 * A318308  Restricted growth sequence transform of A294898.
 * https://oeis.org/A318308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of A294898, where
 *            A294898(n) = 2n - sigma(n) - popcount(n); a(n) = index (1-based)
 *            of A294898(n) among distinct values in order of first
 *            appearance. Computed by a divisor sieve plus a scan
 * Range:     u8 n <= 284, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 111 terms through n = 9822 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A318308_val(seq_t *sg, size_t k)
{
    seq_t pop = (seq_t)__builtin_popcountll((unsigned long long)k);
    return (seq_t)2 * (seq_t)k - sg[k] - pop;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N == 0) return 0;
    seq_t *sg = mem_new(seq_t, N + 1);
    for (size_t d = 1; d <= N; ++d)
        for (size_t m = d; m <= N; m += d)
            sg[m] += (seq_t)d;
    seq_t *dist = mem_new(seq_t, N + 1);
    size_t cnt = 0;
    size_t result = 0;
    for (size_t k = 1; k <= N; ++k) {
        seq_t v = A318308_val(sg, k);
        size_t found = 0;
        for (size_t j = 0; j < cnt; ++j) {
            if (dist[j] == v) { found = j + 1; break; }
        }
        if (!found) {
            dist[cnt++] = v;
            found = cnt;
        }
        result = found;
    }
    mem_free(sg);
    mem_free(dist);
    return (seq_t)result;
}

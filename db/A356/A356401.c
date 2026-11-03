/*
 * A356401  a(n) = n! * Sum_{k=1..n} Sum_{d|k} (-1)^(d+1)/(d * (k/d)!).
 * https://oeis.org/A356401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} Sum_{d|k} (-1)^(d+1)/(d * (k/d)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A356401: a(n) = n! Sum_{k=1..n} Sum_{d|k} (-1)^(d+1)/(d (k/d)!). So a(n) = n a(n-1) + Sum_{d|n} (-1)^(d+1) n!/(d m!) with m = n/d, and n!/(d m!) = m (m+1)(m+2)...(n-1) (for d = 1, 1): products only.
 * Stirling rows by their recurrences and plain products only, so exact modulo
 * the width (the original's caches read entries that realloc had left unset,
 * or divided values that had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t r = (seq_t)0;
    for (size_t m = 1; m <= (size_t)n; ++m) {
        seq_t t = (seq_t)0;
        for (size_t d = 1; d <= m; ++d) {
            if (m % d) continue;
            size_t q = m / d;
            seq_t p = (seq_t)1;
            if (d > 1) {
                p = (seq_t)q;
                for (size_t i = q + 1; i < m; ++i) p *= (seq_t)i;
            }
            if (d & 1) t += p; else t -= p;
        }
        r = (seq_t)m * r + t;
    }
    return r;
}

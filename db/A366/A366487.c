/*
 * A366487  First differences of "commas" sequence A121805.
 * https://oeis.org/A366487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   a(n) = A121805(n+1) - A121805(n), where A121805(1)=1 and
 *            A121805(k) = A121805(k-1) + 10*(A121805(k-1) mod 10) + y, y the
 *            least digit 0..9 equal to the leading digit of the result
 * Range:     u8 n <= 6, u16 n <= 1330, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 115 terms through n = 9941 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A121805_cache = NULL;
static idx_t A121805_max = 0;

static seq_t A121805_msd(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

static seq_t A121805_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > A121805_max) {
        A121805_cache = mem_resize(seq_t, A121805_cache, (size_t)(n + 1));
        if (!A121805_cache) { A121805_max = 0; return (seq_t)0; }
        if (A121805_max == 0) {
            A121805_cache[1] = (seq_t)1;
            A121805_max = 1;
        }
        for (idx_t i = A121805_max + 1; i <= n; ++i) {
            seq_t prev = A121805_cache[i - 1];
            seq_t lsd = prev % (seq_t)10;
            seq_t base = prev + lsd * (seq_t)10;
            seq_t val = 0;
            if (prev != 0) {
                for (seq_t d = 0; d <= 9; ++d) {
                    seq_t cand = base + d;
                    if (A121805_msd(cand) == d) { val = cand; break; }
                }
            }
            A121805_cache[i] = val;
        }
        A121805_max = n;
    }
    return A121805_cache[n];
}

seq_t a(idx_t n)
{
    seq_t hi = A121805_a(n + 1);
    seq_t lo = A121805_a(n);
    if (hi == 0) return (seq_t)0;
    return hi - lo;
}

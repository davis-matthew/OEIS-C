/*
 * A363417  a(n) = Sum_{j=0..2^n - 1} b(j) for n >= 0 where b(n) = (A023416(n)
 *          + 1)*b(A053645(n)) + [A036987(n) = 0]*b(A266341(n)) for n > 0 with
 *          b(0) = 1.
 * https://oeis.org/A363417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..2^n-1} b(j), b(0)=1, b(m) =
 *            (A023416(m)+1)*b(m-2^floor(log2 m)) + [m+1 not a power of
 *            2]*b(A266341(m))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A363417_cache[64];
static int A363417_have = -1;

seq_t a(idx_t n)
{
    if ((int)n <= A363417_have)
        return A363417_cache[n];
    unsigned long long N = 1ULL << n;
    seq_t *b = mem_new(seq_t, (size_t)N);
    seq_t sum = 0;
    int level = 0;
    b[0] = 1;
    sum = 1;
    A363417_cache[0] = 1;
    for (unsigned long long m = 1; m < N; m++) {
        int msb = 0;
        unsigned long long t = m;
        while (t >>= 1) msb++;
        unsigned long long top = 1ULL << msb;
        unsigned long long lower_mask = top - 1;
        unsigned long long lower = m & lower_mask;
        unsigned long long zeros = lower_mask - lower;
        /* zero count of m in binary */
        int zc = 0;
        for (int i = 0; i < msb; i++)
            if (!((m >> i) & 1)) zc++;
        seq_t v = (seq_t)(zc + 1) * b[lower];
        if (((m + 1) & m) != 0) {
            /* not all ones: zeros != 0 */
            int pos = 0;
            unsigned long long z = zeros;
            while (z >>= 1) pos++;
            unsigned long long img = lower + (1ULL << pos);
            v += b[img];
        }
        b[m] = v;
        sum += v;
        if (((m + 1) & m) == 0) {
            level++;
            A363417_cache[level] = sum;
        }
    }
    mem_free(b);
    A363417_have = (int)n;
    return A363417_cache[n];
}

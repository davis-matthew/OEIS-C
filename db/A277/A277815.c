/*
 * A277815  a(n) = the largest k < n for which A264977(k) = A264977(n), or 0
 *          if no such k exists.
 * https://oeis.org/A277815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k < n : A264977(k) = A264977(n)}, or 0 if none;
 *            A264977(0)=0, A264977(1)=1, A264977(2m)=2*A264977(m),
 *            A264977(2m+1)=A264977(m) XOR A264977(m+1) (computed iteratively,
 *            then a backward scan)
 * Range:     u8 n <= 304, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9627 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    seq_t *v = mem_new(seq_t, N + 2);
    v[0] = 0;
    v[1] = 1;
    for (size_t i = 2; i <= N; i++) {
        size_t m = i >> 1;
        if ((i & 1) == 0) v[i] = 2 * v[m];
        else v[i] = v[m] ^ v[m + 1];
    }
    seq_t res = 0;
    seq_t target = v[N];
    for (size_t k = N; k-- > 0; ) {
        if (v[k] == target) { res = (seq_t)k; break; }
    }
    mem_free(v);
    return res;
}

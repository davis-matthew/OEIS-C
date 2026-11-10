/*
 * A347497  Alternating antidiagonal sums of A047920.
 * https://oeis.org/A347497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * T(n-k,k), where T(r,k) =
 *            Sum_{j=0..k} (-1)^j*binomial(k,j)*(r-j)! (A047920 rows),
 *            computed via successive differences of factorials
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 425 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *cur = mem_new(seq_t, N + 1);
    if (!cur) return 0;
    cur[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        cur[i] = cur[i - 1] * (seq_t)i;

    seq_t result = (seq_t)0;
    for (size_t k = 0; k <= N / 2; ++k) {
        if (k > 0) {
            for (size_t r = N; r >= k; --r)
                cur[r] = cur[r] - cur[r - 1];
        }
        seq_t t = cur[N - k];
        if (k & 1) result -= t;
        else result += t;
    }
    mem_free(cur);
    return result;
}

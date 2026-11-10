/*
 * A347431  Antidiagonal sums of A047920.
 * https://oeis.org/A347431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} T(n-k,k), T(r,0) = r!, T(r,k) =
 *            T(r,k-1) - T(r-1,k-1) (A047920 row r, column k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 415 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *f = mem_new(seq_t, N + 1);
    f[0] = 1;
    for (size_t i = 1; i <= N; i++)
        f[i] = f[i - 1] * (seq_t)i;

    seq_t total = f[N];            /* k = 0: T(n,0) = n! */
    for (size_t k = 1; k <= N / 2; k++) {
        for (size_t r = N; r >= k; r--)
            f[r] = f[r] - f[r - 1];
        total += f[N - k];         /* T(n-k, k) */
    }
    mem_free(f);
    return total;
}

/*
 * A327027  T(n, k) = (1/n) * Sum_{d|n} phi(d) * A241171(n/d, k) for n >= 1,
 *          T(0, k) = 0^k. Triangle read by rows for 0 <= k <= n.
 * https://oeis.org/A327027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (1/n) * Sum_{d|n} phi(d) * U(n/d,k), T(0,k) = 0^k,
 *            where U = A241171 in (0,0)-based form: U(0,0) = 1, U(m,0) = 0
 *            for m >= 1, U(m,k) = k*(2k-1)*U(m-1,k-1) + k^2*U(m-1,k)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 40, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static idx_t T327027_phi(idx_t m)
{
    idx_t res = m;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res -= res / p;
        }
    }
    if (m > 1) res -= res / m;
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;
    if (r == 0) return (seq_t)1;

    seq_t *cur = mem_new(seq_t, (size_t)(r + 2));
    cur[0] = 1;
    seq_t sum = 0;
    for (idx_t m = 1; m <= r; ++m) {
        for (idx_t j = m; j >= 1; --j) {
            seq_t c1 = (seq_t)(j * (2 * j - 1));
            seq_t c2 = (seq_t)(j * j);
            cur[j] = c1 * cur[j - 1] + c2 * cur[j];
        }
        cur[0] = 0;
        if (r % m == 0) {
            sum += (seq_t)T327027_phi(r / m) * cur[k];
        }
    }
    sum = sum / (seq_t)r;
    mem_free(cur);
    return sum;
}

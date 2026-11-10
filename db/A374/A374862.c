/*
 * A374862  Obverse convolution A000201**A000201; see Comments.
 * https://oeis.org/A374862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (f(k) + f(n-k)), where f(k) =
 *            floor(k*phi) = A000201(k), f(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-10)
 * Helpers:   isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"
#include "../utils/mem.h"

static seq_t A374862_f(idx_t k)
{
    seq_t K = (seq_t)k;
    if (k == 0) return (seq_t)0;
    seq_t r = isqrt_seq(5 * K * K);
    return (K + r) / 2;
}

seq_t a(idx_t n)
{
    seq_t *f = mem_new(seq_t, (size_t)n + 1);
    for (idx_t i = 0; i <= n; ++i)
        f[i] = A374862_f(i);

    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k)
        prod *= (f[k] + f[n - k]);

    mem_free(f);
    return prod;
}

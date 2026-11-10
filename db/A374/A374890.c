/*
 * A374890  Obverse convolution (n^2)**A000045; see Comments.
 * https://oeis.org/A374890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^2 + F(n-k)), F = A000045 (Fibonacci),
 *            F(0)=0, F(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t *fib = mem_new(seq_t, (size_t)n + 1);
    idx_t i;
    seq_t prod = 1;
    if (!fib) return 0;
    fib[0] = 0;
    if (n >= 1) fib[1] = 1;
    for (i = 2; i <= n; i++)
        fib[i] = fib[i - 1] + fib[i - 2];
    for (i = 0; i <= n; i++) {
        seq_t k = (seq_t)i;
        prod *= (k * k + fib[n - i]);
    }
    mem_free(fib);
    return prod;
}

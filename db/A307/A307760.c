/*
 * A307760  a(n) = (A288840(5*n) - A288840(n))/3000.
 * https://oeis.org/A307760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A288840(5*n) - A288840(n))/3000, where A288840 =
 *            coefficients of E_8/E_6 = (1+480*Sum sigma_7(k) q^k)/(1-504*Sum
 *            sigma_5(k) q^k)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 72 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A288840_table(size_t N)
{
    seq_t *s5  = mem_new(seq_t, N + 1);
    seq_t *s7  = mem_new(seq_t, N + 1);
    seq_t *inv = mem_new(seq_t, N + 1);
    seq_t *ac  = mem_new(seq_t, N + 1);

    for (size_t d = 1; d <= N; ++d) {
        seq_t d5 = 1, d7 = 1;
        for (int i = 0; i < 5; ++i) d5 *= (seq_t)d;
        for (int i = 0; i < 7; ++i) d7 *= (seq_t)d;
        for (size_t m = d; m <= N; m += d) {
            s5[m] += d5;
            s7[m] += d7;
        }
    }
    inv[0] = 1;
    for (size_t k = 1; k <= N; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i <= k; ++i) sum += s5[i] * inv[k - i];
        inv[k] = sum * 504;
    }
    ac[0] = 1;
    for (size_t k = 1; k <= N; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i <= k; ++i) sum += s7[i] * inv[k - i];
        ac[k] = inv[k] + sum * 480;
    }
    mem_free(s5);
    mem_free(s7);
    mem_free(inv);
    return ac;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_N = 0;
    size_t N = (size_t)n * 5;
    if (!cache || N > cached_N) {
        if (cache) mem_free(cache);
        cache = A288840_table(N);
        cached_N = N;
    }
    seq_t hi = cache[N];
    seq_t lo = cache[(size_t)n];
    return (hi - lo) / 3000;
}

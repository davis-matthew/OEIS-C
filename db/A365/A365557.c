/*
 * A365557  Expansion of e.g.f. 1 / (7 - 6 * exp(x))^(5/6).
 * https://oeis.org/A365557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=1..n} (6 - k/n) * binomial(n,k) * a(n-k)
 *            (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 10000  /* assumes no query larger than this value */

seq_t a(idx_t n)
{
    static seq_t cache[MAX_N + 1];
    static idx_t computed = 0;
    static int   init_flag = 0;

    if (!init_flag) {
        cache[0] = (seq_t)1;
        init_flag = 1;
    }

    if (n <= computed)
        return cache[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;                      /* binomial(i,k) iterated */
        for (idx_t k = 1; k <= i; ++k) {
            /* bin = binomial(i,k) */
            bin = bin * (seq_t)(i - k + 1);
            bin = bin / (seq_t)k;

            seq_t term = cache[i - k] * bin;
            term = term * (seq_t)(6 * i - k);
            sum += term;
        }
        cache[i] = sum / (seq_t)i;
    }

    computed = n;
    return cache[n];
}

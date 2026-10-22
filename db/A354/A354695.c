/*
 * A354695  G.f. A(x) satisfies A(x) = 1 + x * A(x^3/(1 - x)^3) / (1 - x)^3.
 * https://oeis.org/A354695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n+1,3*k+2) * a(k);
 *            a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq_cache = NULL;
    static size_t seq_len  = 0;  /* number of values stored (next free index) */
    static size_t seq_cap  = 0;  /* current capacity of seq_cache */

    /* Initialise cache with a(0) */
    if (seq_cache == NULL) {
        seq_cap = (size_t)n + 1;
        if (seq_cap == 0) seq_cap = 1;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        seq_cache[0] = (seq_t)1;
        seq_len = 1;
    }

    /* If the requested index is already computed, just return it */
    if (n < seq_len)
        return seq_cache[n];

    /* Grow capacity if necessary */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Compute values from seq_len up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        if (i == 0) {
            seq_cache[0] = (seq_t)1;   /* already set, but keep for safety */
            continue;
        }

        idx_t N      = i + 1;
        idx_t kmax   = (i == 0) ? 0 : (i - 1) / 3;
        seq_t sum    = 0;
        seq_t b      = (seq_t)N * (seq_t)(N - 1) / 2;  /* binom(N,2) for k=0 */

        sum += b * seq_cache[0];

        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t mprev = 3 * (k - 1) + 2;
            for (int step = 1; step <= 3; ++step) {
                idx_t mnew = mprev + step;
                b = b * (seq_t)(N - (mnew - 1)) / (seq_t)mnew;
            }
            sum += b * seq_cache[k];
        }

        seq_cache[i] = sum;
    }

    seq_len = (size_t)n + 1;
    return seq_cache[n];
}

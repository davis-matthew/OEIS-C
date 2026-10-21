/*
 * A258279  Expansion of psi(q)^2 * chi(-q^3)^2 in powers of q where psi(),
 *          chi() are Ramanujan theta functions.
 * https://oeis.org/A258279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the period‑6 sequence * b(k) =
 *            [2, -2, 0, -2, 2, -2] repeating. * Recurrence: * a(0) = 1 * For
 *            n ≥ 1: * a(n) = (1/n) * Σ_{k=1..n} c(k) * a(n‑k) * where c(k) =
 *            Σ_{d|k} d * b(d) (sum over divisors of k) * * This follows from
 *            the identity: * Σ a(n) x^n = ∏_{k≥1} (1 - x...
 * Range:     i8 n <= 24, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Work with a size_t copy of the index; all loops use size_t to
       avoid signed/unsigned mismatch. */
    size_t N = (size_t)n;

    /* Static cache of sequence values.  In C++ builds we use a vector;
       in C builds we use a raw array. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.size() < N + 1) {
        cache.clear();
        cache.resize(N + 1);
        /* Allocate temporary array for c(k). */
        std::vector<seq_t> c(N + 1, 0);
        /* Compute b(d) and accumulate divisor sums into c. */
        for (size_t d = 1; d <= N; ++d) {
            int b;
            switch ((d - 1) % 6) {
                case 0: b =  2; break;
                case 1: b = -2; break;
                case 2: b =  0; break;
                case 3: b = -2; break;
                case 4: b =  2; break;
                default: b = -2; break;  /* case 5 */
            }
            seq_t delta = (seq_t)d * (seq_t)b;
            for (size_t m = d; m <= N; m += d)
                c[m] += delta;
        }
        /* Compute sequence values using the recurrence. */
        cache[0] = 1;
        for (size_t i = 1; i <= N; ++i) {
            seq_t sum = 0;
            for (size_t k = 1; k <= i; ++k)
                sum += c[k] * cache[i - k];
            cache[i] = sum / (seq_t)i;
        }
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache == NULL || cache_size < N + 1) {
        /* Allocate new cache, free old. */
        seq_t *new_cache = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (cache != NULL)
            free(cache);
        cache = new_cache;
        cache_size = N + 1;

        /* Temporary array for divisor sums. */
        seq_t *c = (seq_t*)calloc(N + 1, sizeof(seq_t));
        for (size_t d = 1; d <= N; ++d) {
            int b;
            switch ((d - 1) % 6) {
                case 0: b =  2; break;
                case 1: b = -2; break;
                case 2: b =  0; break;
                case 3: b = -2; break;
                case 4: b =  2; break;
                default: b = -2; break;  /* case 5 */
            }
            seq_t delta = (seq_t)d * (seq_t)b;
            for (size_t m = d; m <= N; m += d)
                c[m] += delta;
        }

        /* Compute sequence values. */
        cache[0] = 1;
        for (size_t i = 1; i <= N; ++i) {
            seq_t sum = 0;
            for (size_t k = 1; k <= i; ++k)
                sum += c[k] * cache[i - k];
            cache[i] = sum / (seq_t)i;
        }
        free(c);
    }
    return cache[n];
#endif
}

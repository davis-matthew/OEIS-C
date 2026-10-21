/*
 * A279735  Number of n X 2 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4) for n ≥ 5, with
 *            a(1)=0, a(2)=2, a(3)=8, a(4)=26
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    size_t N = (size_t)n;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t computed = 0;

    /* Initial allocation of at least the first four terms */
    if (cache == NULL) {
        size_t init_len = (N > 4 ? N : 4);
        cache = (seq_t*)malloc(init_len * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
        cache[0] = 0;              /* a(1) */
        cache[1] = 2;              /* a(2) */
        cache[2] = 8;              /* a(3) */
        cache[3] = 26;             /* a(4) */
        cache_len = init_len;
        computed = 4;
    }

    /* Extend cache and compute any missing terms */
    if (N > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, N * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache = tmp;
        cache_len = N;

        /* Compute new terms using the recurrence */
        for (size_t i = computed + 1; i <= N; ++i) {
            seq_t a1 = cache[i - 2];   /* a(i-1) */
            seq_t a2 = cache[i - 3];   /* a(i-2) */
            seq_t a3 = cache[i - 4];   /* a(i-3) */
            seq_t a4 = cache[i - 5];   /* a(i-4) */
            cache[i - 1] = 6 * a1 - 11 * a2 + 6 * a3 - a4;
        }
        computed = N;
    }

    return cache[N - 1];
}

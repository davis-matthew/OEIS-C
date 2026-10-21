/*
 * A297334  Number of n X 4 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 10*a(n-2) - 4*a(n-3) + 3*a(n-4) + a(n-5)
 *            Initial values: a(1)=8, a(2)=53, a(3)=334, a(4)=2119, a(5)=13428
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 10000          /* pre‑allocate space for n up to about 10 000 */

static seq_t cache[MAX_N + 1];
static size_t cache_len = 0;   /* highest index that has been computed */

/* Return the n‑th term of A297334, where a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* initialise the first five terms once */
    if (cache_len == 0) {
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)53;
        cache[3] = (seq_t)334;
        cache[4] = (seq_t)2119;
        cache[5] = (seq_t)13428;
        cache_len = 5;
    }

    /* If the value has already been computed, return it. */
    if ((size_t)n <= cache_len) return cache[(size_t)n];

    /* Compute further terms up to n, staying inside the static array
       if n ≤ MAX_N. */
    if ((size_t)n <= MAX_N) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] * (seq_t)8
                       - cache[i-2] * (seq_t)10
                       - cache[i-3] * (seq_t)4
                       + cache[i-4] * (seq_t)3
                       + cache[i-5];
        }
        cache_len = (size_t)n;
        return cache[(size_t)n];
    }

    /* For n > MAX_N, recompute using a sliding five‑term window
       based on the last five cached values. */
    seq_t prev[5];
    size_t start = cache_len - 4;                 /* cache_len ≥ 5 */
    for (size_t k = 0; k < 5; ++k)
        prev[k] = cache[start + k];

    seq_t cur = 0;
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cur = prev[4] * (seq_t)8
              - prev[3] * (seq_t)10
              - prev[2] * (seq_t)4
              + prev[1] * (seq_t)3
              + prev[0];

        /* shift the window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = cur;
    }
    return cur;
}

/*
 * A261058  Column k=1 of A213221.
 * https://oeis.org/A261058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (10*(2*n-5)*a(n-6) + 2*(2*n-3)*a(n-5) + 3*(7*n-10)*a(n-2)
 *             - ( (n-3)*a(n-1) + 3*(n-1)*a(n-3) + 40*(n-2)*a(n-4) ) ) / n
 * Initial values: a(1)=1, a(2)=3, a(3)=10, a(4)=32, a(5)=106, a(6)=357
 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static idx_t max_computed = 0;

    /* Allocate and initialise cache for the first call or if n expands the size */
    if (cache == NULL) {
        cache_capacity = 7;                     /* indices 0..6 */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        if (!cache) return (seq_t)0;            /* allocation failure */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)32;
        cache[5] = (seq_t)106;
        cache[6] = (seq_t)357;
        max_computed = 6;                       /* we have values up to 6 */
    }

    /* For small n just return the pre‑computed value */
    if (n <= 6) {
        if (n < 1) return (seq_t)0;            /* not part of the defined offset */
        return cache[n];
    }

    /* Resize cache if necessary */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;             /* allocation failure */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute forward from the last computed term up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = (seq_t)(i - 3) * cache[i - 1];
        pos += (seq_t)(3 * (i - 1)) * cache[i - 3];
        pos += (seq_t)(40 * (i - 2)) * cache[i - 4];

        /* Absolute values of the negative contributions */
        seq_t neg = (seq_t)(3 * (7 * i - 10)) * cache[i - 2];
        neg += (seq_t)(2 * (2 * i - 3)) * cache[i - 5];
        neg += (seq_t)(10 * (2 * i - 5)) * cache[i - 6];

        seq_t diff = neg - pos;                /* neg > pos for i >= 7 */
        cache[i] = diff / (seq_t)i;
    }

    max_computed = n;
    return cache[n];
}

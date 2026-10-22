/*
 * A336502  Partial sums of A057003.
 * https://oeis.org/A336502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} Product_{j=T(i-1)+1..T(i)} j * where T(k) =
 *            k*(k+1)/2 is the k-th triangular number
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *a_cache = NULL; /* a_cache[i] = a(i) */
static size_t cache_len = 0;   /* largest i for which a_cache[i] is valid */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defensive; offset is 1 */

    /* Allocate cache on first call */
    if (a_cache == NULL) {
        a_cache = (seq_t*)malloc(sizeof(seq_t) * (size_t)(n + 1));
        if (!a_cache) return (seq_t)0;    /* allocation failure */
        a_cache[0] = (seq_t)0;
        cache_len = 0;
    }

    /* If already computed, return directly */
    if ((idx_t)cache_len >= n) {
        return a_cache[(size_t)n];
    }

    /* current triangular number before the first new block */
    seq_t cur = (seq_t)cache_len * ((seq_t)cache_len + (seq_t)1) / (seq_t)2;
    seq_t sum = a_cache[(size_t)cache_len];

    /* Resize cache to hold up to n if necessary */
    if ((size_t)n > cache_len) {
        seq_t *tmp = (seq_t*)realloc(a_cache, sizeof(seq_t) * ((size_t)n + 1));
        if (!tmp) { /* allocation failed; return last computed sum */
            return sum;
        }
        a_cache = tmp;
    }

    /* Compute blocks from cache_len+1 up to n */
    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        seq_t block = (seq_t)1;
        idx_t start = (idx_t)(cur + (idx_t)1);
        idx_t end   = (idx_t)(cur + i);
        for (idx_t j = start; j <= end; ++j) {
            block *= (seq_t)j;
        }
        sum += block;
        cur = end;
        a_cache[(size_t)i] = sum;
    }

    cache_len = (size_t)n;
    return sum;
}

/*
 * A249512  Expansion of 1/(1-x*sqrt(4*x^2+1)-2*x^2).
 * https://oeis.org/A249512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (-n+1)*a(n)+(-n+2)*a(n-1)+(n+11)*a(n-2)+(n+10)*a(n-3)+20*(n-4)*a(n-4)+20*(n-5)*a(n-5)=0,
   a(0)=1, a(1)=1, a(2)=3, a(3)=7, a(4)=15 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)15;
    }
    if (n < cache_len) {
        return cache[n];
    }
    /* Expand cache if necessary */
    size_t needed = n + 1;
    if (needed > cache_len) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        cache_len = needed;
    }
    for (idx_t j = (idx_t)5; j <= n; ++j) {
        if (j < 5) continue;  /* already initialized */
        seq_t sumpos =
            ((seq_t)(j + 11)) * cache[j - 2] +
            ((seq_t)(j + 10)) * cache[j - 3] +
            ((seq_t)20 * (j - 4)) * cache[j - 4] +
            ((seq_t)20 * (j - 5)) * cache[j - 5];
        seq_t term1 = ((seq_t)(j - 2)) * cache[j - 1];
        seq_t num = sumpos - term1;
        seq_t denom = (seq_t)(j - 1);
        cache[j] = num / denom;
    }
    return cache[n];
}

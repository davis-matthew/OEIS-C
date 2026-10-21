/*
 * A246038  G.f.: (1+2x)(1+2x+4x^2)/(1-3x-8x^3-8x^4).
 * https://oeis.org/A246038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (1+2x)(1+2x+4x^2)/(1-3x-8x^3-8x^4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(2)=29, a(3)=103
   For n>=4: a(n)=3*a(n-1)+8*a(n-3)+8*a(n-4)+ (4 if n==1; 8 if n==2||n==3; 0 otherwise) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp && n + 1 > 0) {
            /* allocation failure – abort */
            abort();
        }
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
                continue;
            }
            seq_t val = 0;
            val += (seq_t)3 * cache[i - 1];
            if (i >= 3) val += (seq_t)8 * cache[i - 3];
            if (i >= 4) val += (seq_t)8 * cache[i - 4];
            if (i == 1) val += 4;
            else if (i == 2 || i == 3) val += 8;
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}

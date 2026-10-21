/*
 * A296397  a(n) = a(n-1) * a(n-2) + a(n-3) * Product_{k=0..n-4} a(k)^2, with
 *          a(0) = a(1) = 1, a(2) = 2.
 * https://oeis.org/A296397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1,a(1)=1,a(2)=2,a(3)=3 and for n≥4:
 *            a(n)=a(n-1)*a(n-2)+a(n-3)*∏_{k=0}^{n-4}a(k)^2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_val = NULL;
    static seq_t *P     = NULL;
    static idx_t cache = 0;          /* number of computed terms */

    if (cache == 0) {                      /* initial allocation */
        cache = 4;                         /* at least indices 0..3 */
        a_val = (seq_t *)malloc(cache * sizeof(seq_t));
        P     = (seq_t *)malloc(cache * sizeof(seq_t));
        if (!a_val || !P) abort();

        /* base cases */
        a_val[0] = (seq_t)1;  P[0] = a_val[0] * a_val[0];
        a_val[1] = (seq_t)1;  P[1] = P[0] * a_val[1] * a_val[1];
        a_val[2] = (seq_t)2;  P[2] = P[1] * a_val[2] * a_val[2];
        a_val[3] = (seq_t)3;  P[3] = P[2] * a_val[3] * a_val[3];
    }

    if (n < cache) return a_val[n];  /* already computed */

    /* grow cache to at least n+1 terms */
    idx_t new_size = n + 1;
    if (new_size > cache) {
        a_val = (seq_t *)realloc(a_val, new_size * sizeof(seq_t));
        P     = (seq_t *)realloc(P, new_size * sizeof(seq_t));
        if (!a_val || !P) abort();
        /* compute missing terms */
        for (idx_t i = cache; i < new_size; ++i) {
            if (i >= 4) {
                a_val[i] = a_val[i-1] * a_val[i-2] + a_val[i-3] * P[i-4];
            }
            P[i] = P[i-1] * a_val[i] * a_val[i];
        }
        cache = new_size;
    }

    return a_val[n];
}

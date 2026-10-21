/*
 * A249531  Number of length 1+5 0..n arrays with no six consecutive terms
 *          having five times any element equal to the sum of the remaining
 *          five.
 * https://oeis.org/A249531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249531: Number of length 1+5 0..n arrays with no six consecutive terms having five times any element equal to the sum of the remaining five.
   Empirical linear recurrence (order 16) with initial terms a(1)=62, a(2)=606, …, a(16)=22716332.
   For n > 16:
     a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) - 2*a(n-6) + 2*a(n-7)
            - 2*a(n-9) + 2*a(n-10) - 3*a(n-11) + 4*a(n-12) - 5*a(n-13) + 6*a(n-14)
            - 4*a(n-15) + a(n-16) */
#include <stdlib.h>

static const seq_t init[16] = {
    (seq_t)62,   (seq_t)606,   (seq_t)3492,   (seq_t)13580,
    (seq_t)40950,  (seq_t)104562, (seq_t)235196, (seq_t)480912,
    (seq_t)911490, (seq_t)1625210, (seq_t)2754672, (seq_t)4474896,
    (seq_t)7011302, (seq_t)10648350, (seq_t)15739200, (seq_t)22716332
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;

static void ensure_cap(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behavior below offset is unspecified */

    if (n > last_computed) {
        ensure_cap(n);
        /* compute missing terms */
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            if (k <= 16) {
                cache[k-1] = init[k-1];
                continue;
            }
            /* compute using unsigned arithmetic; the recurrence guarantees
               that the positive sum is always ≥ the negative sum, so
               the unsigned subtraction will not underflow. */
            seq_t pos = 0, neg = 0;

            /* positive coefficients */
            pos += (seq_t)4  * cache[k-2];   /* a(k-1)  */
            pos += (seq_t)5  * cache[k-4];   /* a(k-3)  */
            pos += (seq_t)3  * cache[k-6];   /* a(k-5)  */
            pos += (seq_t)2  * cache[k-8];   /* a(k-7)  */
            pos += (seq_t)2  * cache[k-11];  /* a(k-10) */
            pos += (seq_t)4  * cache[k-13];  /* a(k-12) */
            pos += (seq_t)6  * cache[k-15];  /* a(k-14) */
            pos +=           cache[k-17];    /* a(k-16) */

            /* negative coefficients */
            neg += (seq_t)6  * cache[k-3];   /* a(k-2)  */
            neg += (seq_t)4  * cache[k-5];   /* a(k-4)  */
            neg += (seq_t)2  * cache[k-7];   /* a(k-6)  */
            neg += (seq_t)2  * cache[k-10];  /* a(k-9)  */
            neg += (seq_t)3  * cache[k-12];  /* a(k-11) */
            neg += (seq_t)5  * cache[k-14];  /* a(k-13) */
            neg += (seq_t)4  * cache[k-16];  /* a(k-15) */

            cache[k-1] = pos - neg;
        }
        last_computed = n;
    }
    return cache[n-1];
}

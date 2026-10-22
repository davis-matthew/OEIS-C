/*
 * A395206  a(n) = (2*n)! * [x^(2*n)] (f(x)^4 + f(-x)^4)/2 and f(x) = 1/sqrt(1
 *          + 2*sqrt(2)*x + x^2).
 * https://oeis.org/A395206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * [x^(2*n)] (1+10*x^2+x^4)/(1-6*x^2+x^4)^2 Let
 *            r(n) = a(n)/(2*n)!. The rational coefficients r(n) satisfy
 *            r(0)=1, r(1)=22, r(2)=227, r(3)=1900
 *            r(n)=12*r(n-1)-38*r(n-2)+12*r(n-3)-r(n-4) for n>=4. Then a(n) =
 *            (2*n)! * r(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers supplied by the build */
#include <stdlib.h>
#include <stddef.h>

/* Function to compute a(n) */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;        /* largest n already computed */
    static seq_t factorial = 1;       /* (2*computed)! */
    static seq_t r_buf[4] = {1, 22, 227, 1900}; /* most recent 4 r-values,
                                                   r_buf[0]=r_{k-4}, r_buf[3]=r_{k-1} */

    /* Helper to grow cache array */
    if (cache == NULL) {
        cache_cap = (size_t)(n < 4 ? 4 : n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        factorial = (seq_t)1;          /* (2*0)! = 1 */
        computed = 0;
        /* r_buf already initialized above */
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Update factorial to (2*i)! from (2*i-2)! */
        factorial *= (seq_t)(2 * i - 1);
        factorial *= (seq_t)(2 * i);

        seq_t r_i;
        if (i <= 3) {            /* use known initial values */
            switch (i) {
                case 1: r_i = (seq_t)22; break;
                case 2: r_i = (seq_t)227; break;
                case 3: r_i = (seq_t)1900; break;
                default: r_i = (seq_t)0;  /* should never happen */
            }
        } else {
            /* Compute next r using the linear recurrence */
            r_i = (seq_t)12 * r_buf[3];
            r_i -= (seq_t)38 * r_buf[2];
            r_i += (seq_t)12 * r_buf[1];
            r_i -= r_buf[0];

            /* Shift the buffer for the next iteration */
            r_buf[0] = r_buf[1];
            r_buf[1] = r_buf[2];
            r_buf[2] = r_buf[3];
            r_buf[3] = r_i;
        }

        /* a(i) = (2*i)! * r_i */
        cache[i] = factorial * r_i;
    }

    computed = n;
    return cache[n];
}

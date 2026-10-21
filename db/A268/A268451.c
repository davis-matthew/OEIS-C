/*
 * A268451  Number of length-n 0..2 arrays with no adjacent pair x,x+1
 *          followed at any distance by x+1,x.
 * https://oeis.org/A268451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 14*a(n-3) - 10*a(n-4) + 4*a(n-5) -
 *            a(n-6) Initial values (n = 1 … 6): a(1)=3, a(2)=9, a(3)=25,
 *            a(4)=67, a(5)=176, a(6)=456 - a(1) is the first term listed in
 *            the OEIS entry (offset 1). - The function memoises computed
 *            terms and evaluates the recurrence for all n ≥ 7....
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    if (n == 0)        /* defensive, not used for offset 1 */
        return 0;

    static seq_t *cache = NULL;   /* array of cached a(i) values, index 0 => a(1) */
    static size_t cache_len = 0;  /* number of elements currently initialized */

    /* Desired size for the cache: need indices 0 … n-1 */
    size_t needed = (size_t)n;
    size_t old_len = cache_len;

    if (cache == NULL) {                  /* first call */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) exit(1);              /* out of memory */
    }
    else if (needed > cache_len) {          /* grow cache if necessary */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }

    /* Compute any missing terms starting from old_len up to needed-1 */
    for (size_t i = old_len; i < needed; ++i) {
        if (i < 6) { /* base cases: a(1) … a(6)  (index 0 … 5) */
            static const seq_t base_vals[6] = { (seq_t)3, (seq_t)9,
                                                (seq_t)25, (seq_t)67,
                                                (seq_t)176, (seq_t)456 };
            cache[i] = base_vals[i];
        } else { /* recurrence for i ≥ 6 (i = n-1 >= 6 => n ≥ 7) */
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            seq_t t6 = cache[i-6];
            /* compute 6*a(n-1) - 13*a(n-2) + 14*a(n-3) - 10*a(n-4) + 4*a(n-5) - a(n-6) */
            seq_t pos = (seq_t)6 * t1;
            pos += (seq_t)14 * t3;
            pos += (seq_t)4 * t5;
            seq_t neg = (seq_t)13 * t2;
            neg += (seq_t)10 * t4;
            neg +=        t6;
            cache[i] = pos - neg;          /* unsigned subtraction: pos >= neg for n ≥ 7 */
        }
    }

    cache_len = needed;                    /* now cache contains a(1)…a(n) */
    return cache[n-1];                      /* n-1 is the 0‑based index of a(n) */
}

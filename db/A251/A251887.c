/*
 * A251887  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not 2 3 6 or 7 and every diagonal and antidiagonal sum
 *          2 3 6 or 7.
 * https://oeis.org/A251887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-3) + a(n-5) - 8*a(n-6) - 4*a(n-8) + 4*a(n-9) +
 *            5*a(n-11) - 2*a(n-14) for n > 22, with initial values:
 *            a(1)=1280, a(2)=1664, a(3)=1280, a(4)=1280, a(5)=1534,
 *            a(6)=1752, a(7)=2146, a(8)=2838, a(9)=3446, a(10)=4406,
 *            a(11)=5830, a(12)=7266, a(13)=9446, a(14)=12606, a(15)=15958,
 *            a(16)=209...
 * Range:     u8 none, u16 n <= 20, u32 n <= 60, u64 n <= 139, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t init_values[23] = {
    (seq_t)0,
    (seq_t)1280, (seq_t)1664, (seq_t)1280, (seq_t)1280, (seq_t)1534,
    (seq_t)1752, (seq_t)2146, (seq_t)2838, (seq_t)3446, (seq_t)4406,
    (seq_t)5830, (seq_t)7266, (seq_t)9446, (seq_t)12606, (seq_t)15958,
    (seq_t)20914, (seq_t)27986, (seq_t)35746, (seq_t)47062, (seq_t)62958,
    (seq_t)80926, (seq_t)106810
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n = 0 */

    /* Ensure storage for indices 0..n */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        /* zero the newly allocated part */
        for (idx_t i = cache_len; i < new_len; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute all values up to n */
    for (idx_t k = 1; k <= n; ++k) {
        if (cache[k] != (seq_t)0) continue;    /* already computed */
        if (k <= 22) {
            cache[k] = init_values[k];
        } else {
            cache[k] = (5*cache[k-3] + cache[k-5]
                        - 8*cache[k-6] - 4*cache[k-8]
                        + 4*cache[k-9] + 5*cache[k-11]
                        - 2*cache[k-14]);
        }
    }
    return cache[n];
}

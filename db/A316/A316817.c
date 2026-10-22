/*
 * A316817  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 5*a(n‑1) + 4*a(n‑2)
 *            + 9*a(n‑3) -34*a(n‑4) + 20*a(n‑5) -5*a(n‑6) +4*a(n‑7) -24*a(n‑8)
 *            -34*a(n‑9) -16*a(n‑10) -8*a(n‑11) for n > 12, with initial
 *            values a(1)=4, a(2)=30, a(3)=169, a(4)=983, a(5)=5701,
 *            a(6)=33046, a(7)=191692, a(8)=1111756, a(9)=6447926, a...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[12] = {
    (seq_t)4,          (seq_t)30,          (seq_t)169,
    (seq_t)983,        (seq_t)5701,        (seq_t)33046,
    (seq_t)191692,     (seq_t)1111756,     (seq_t)6447926,
    (seq_t)37397230,   (seq_t)216897344,   (seq_t)1257968840
};

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure cache contains at least n terms (a(1) … a(n)) */
static void ensure_cache(idx_t n)
{
    if (n <= 0) return;
    /* n fits in size_t for our usage */
    size_t need = (size_t)n;
    if (need <= cache_len) return;              /* already have enough terms */

    size_t new_len = need;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) return;                      /* allocation failure – caller will get 0 */

    /* copy any existing terms */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* initialise the first 12 terms if not already initialised */
    size_t upto = (new_len < 12) ? new_len : 12;
    for (size_t i = cache_len; i < upto; ++i) new_cache[i] = init_vals[i];

    /* compute remaining terms using the recurrence */
    size_t start = (cache_len < 12) ? 12 : cache_len;
    for (size_t k = start; k < new_len; ++k) {
        seq_t val =  5 * new_cache[k - 1]
                    +4 * new_cache[k - 2]
                    +9 * new_cache[k - 3]
                    -34 * new_cache[k - 4]
                    +20 * new_cache[k - 5]
                    -5  * new_cache[k - 6]
                    +4  * new_cache[k - 7]
                    -24 * new_cache[k - 8]
                    -34 * new_cache[k - 9]
                    -16 * new_cache[k -10]
                    -8  * new_cache[k -11];
        new_cache[k] = val;
    }

    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined; return 0 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}

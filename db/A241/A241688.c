/*
 * A241688  Number of Sidon subsets of {1,...,n} of size 4.
 * https://oeis.org/A241688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of Sidon subsets of size 4 of {1,…,n} Linear
 *            recurrence: a(n) =
 *            a(n-11)+a(n-8)-a(n-3)+2*(a(n-6)+a(n-1)-a(n-10)-a(n-5)) (valid
 *            for n ≥ 12) with initial values a(1)…a(11) listed below. Initial
 *            values: a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=0,a(7)=2,
 *            a(8)=10,a(9)=26,a(10)=60,a(11)=110
 * Range:     u8 n <= 12, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {0,0,0,0,0,0,0,2,10,26,60,110};

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 11;        /* highest index currently computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 12;                  /* at least up to index 11 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return 0;      /* allocation failure (unlikely) */
        for (idx_t i = 1; i <= 11; ++i) cache[i] = init[i];
    }

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;   /* double to reduce reallocs */
        if (new_size < n + 1) new_size = n + 1;   /* guard overflow */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return 0;      /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    if (n <= computed_max) {            /* value already known */
        return cache[n];
    }

    /* compute up to n using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = cache[i-11] + cache[i-8];
        pos = pos + cache[i-6] + cache[i-1] + cache[i-6] + cache[i-1]; /* +2*(a[i-6]+a[i-1]) */
        seq_t neg = cache[i-3] + cache[i-10] + cache[i-5] + cache[i-10] + cache[i-5]; /* +2*(a[i-10]+a[i-5]) */
        cache[i] = pos - neg;           /* safe because pos ≥ neg for all i≥12 */
    }
    computed_max = n;

    return cache[n];
}

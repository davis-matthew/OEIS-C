/*
 * A374737  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 1 or 5.
 * https://oeis.org/A374737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)-a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-10)
 *            (for n ≥ 10). Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=5,
 *            a(4)=8, a(5)=13, a(6)=18, a(7)=28, a(8)=41, a(9)=62
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {1,2,3,5,8,13,18,28,41,62};

static seq_t *cache = NULL;          /* dynamic array of computed terms   */
static idx_t  cache_cap   = 0;        /* allocated capacity (number of slots) */
static idx_t  computed_upto = 0;      /* largest index that has been filled   */

seq_t a(idx_t n)
{
    /* Make sure capacity covers index n */
    if (cache == NULL) {
        cache_cap = (n+1 > 10 ? n+1 : 10);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;          /* allocation failed */
        for (idx_t i=0;i<10;i++)             /* initial values   */
            cache[i] = init_vals[i];
        for (idx_t i=10;i<cache_cap;i++)
            cache[i] = 0;                     /* unused slots zeroed */
        computed_upto = 9;
    } else if (n+1 > cache_cap) {
        idx_t new_cap = n+1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return 0;          /* allocation failed */
        cache = tmp;
        for (idx_t i=cache_cap;i<new_cap;i++)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* If the requested term has already been computed, return it */
    if (n <= computed_upto)
        return cache[n];

    /* Compute missing terms from computed_upto + 1 up to n (using the recurrence) */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t sum = cache[i-2] + cache[i-3] + cache[i-4] +
                    cache[i-6] + cache[i-7] + cache[i-8] +
                    cache[i-10];
        cache[i] = sum - cache[i-5];
    }
    computed_upto = n;

    return cache[n];
}

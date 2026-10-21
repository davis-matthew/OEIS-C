/*
 * A296278  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1)*b(n), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          b(2) = 5, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1)*b(n); a(0)=1, a(1)=2;
 *            b(0)=3, b(1)=4, b(2)=5
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 81, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *a_cache = NULL;      /* cached a(n) values          */
static seq_t *b_cache = NULL;      /* cached b(n) values          */
static size_t cap_a = 0;           /* allocated size of a_cache   */
static size_t cap_b = 0;           /* allocated size of b_cache   */
static idx_t  computed = 0;        /* number of terms already computed */

/* binary search: return 1 if v occurs in arr[0..cnt-1], else 0 */
static int contains(const seq_t *arr, idx_t cnt, seq_t v)
{
    idx_t lo = 0, hi = cnt;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (arr[mid] == v) return 1;
        if (arr[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

/* ensure the cache has space for at least need elements */
static void grow_cache(seq_t **cache, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure */
    *cache = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialisation: first three terms */
    if (computed == 0) {
        grow_cache(&a_cache, &cap_a, 3);
        grow_cache(&b_cache, &cap_b, 3);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_cache[2] = (seq_t)63;   /* 1+2+3*4*5 */
        b_cache[0] = (seq_t)3;
        b_cache[1] = (seq_t)4;
        b_cache[2] = (seq_t)5;
        computed = 3;
    }

    if ((idx_t)n < computed)
        return a_cache[n];

    grow_cache(&a_cache, &cap_a, (size_t)n + 1);
    grow_cache(&b_cache, &cap_b, (size_t)n + 1);

    for (idx_t i = computed; i <= (idx_t)n; ++i) {
        /* compute b(i) : smallest integer > b(i‑1) not in a[0..i‑1] */
        seq_t val = b_cache[i - 1] + (seq_t)1;
        while (contains(a_cache, i, val))
            ++val;
        b_cache[i] = val;

        /* compute a(i) via the recurrence */
        seq_t prod = b_cache[i - 2];
        prod *= b_cache[i - 1];
        prod *= b_cache[i];
        a_cache[i] = a_cache[i - 1] + a_cache[i - 2] + prod;
    }
    computed = (idx_t)n + 1;
    return a_cache[n];
}

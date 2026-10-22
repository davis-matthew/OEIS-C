/*
 * A302011  Number of 4 X n 0..1 arrays with every element equal to 0, 1, 2 or
 *          3 horizontally or antidiagonally adjacent elements, with upper
 *          left element zero.
 * https://oeis.org/A302011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) + 20*a(n-2) - 16*a(n-3) - 64*a(n-4) * for n >
 *            6, with initial values * a(1)=8, a(2)=128, a(3)=1808,
 *            a(4)=25808, a(5)=369040, a(6)=5276816
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 */
    if (n == 0) return 0;          /* harmless fallback for offset */

    /* static memoisation array (1‑based) */
    static seq_t *cache = NULL;    /* pointer to array elements a[0..] */
    static idx_t cache_cap = 0;    /* size allocated in elements */
    static idx_t computed_max = 0; /* largest index that is known */

    /* Ensure we have room for indices up to max(n,6) */
    idx_t need = n > 6 ? n + 1 : 7;  /* at least 0..6 must exist */
    if (cache_cap < (idx_t)need) {
        size_t new_cap = (size_t)need;
        /* cast the void* from realloc to compile in C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – return 0 */
        cache = tmp;
        cache_cap = need;
    }

    /* initialise the first six values once */
    if (computed_max < 6) {
        cache[0] = (seq_t)0;          /* unused slot for 1‑based access */
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)128;
        cache[3] = (seq_t)1808;
        cache[4] = (seq_t)25808;
        cache[5] = (seq_t)369040;
        cache[6] = (seq_t)5276816;
        computed_max = 6;
    }

    /* compute any missing terms up to n using the recurrence */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            /* avoid overflow with unsigned types by computing in two steps */
            seq_t left  = cache[i-1] * (seq_t)13 + cache[i-2] * (seq_t)20;
            seq_t right = cache[i-3] * (seq_t)16 + cache[i-4] * (seq_t)64;
            cache[i] = left - right;
        }
        computed_max = n;
    }

    return cache[n];
}

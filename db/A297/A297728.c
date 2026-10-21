/*
 * A297728  Number of nX3 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A297728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) +13*a(n-3) -2*a(n-4) +4*a(n-5) -11*a(n-6)
 *            -3*a(n-7) -a(n-8) with
 *            a(1)=1,a(2)=8,a(3)=17,a(4)=39,a(5)=151,a(6)=385,a(7)=1026,a(8)=3272
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Ensure that cache can hold indices 0..required (inclusive) */
static void ensure_capacity(idx_t required, seq_t **arr, size_t *cap)
{
    /* We also need space up to index 8 for base initialization.  */
    if (required < 8)
        required = 8;

    if (*cap <= required) {
        size_t new_cap = (*cap > 0) ? *cap : 1;
        while (new_cap <= required)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, (new_cap + 1) * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* value undefined for n = 0 */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;      /* largest index computed (>=0) */

    /* Expand cache to accommodate base values (indices 1..8) and requested n. */
    ensure_capacity(n, &cache, &cap);

    /* Initialise base terms once. */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)39;
        cache[5] = (seq_t)151;
        cache[6] = (seq_t)385;
        cache[7] = (seq_t)1026;
        cache[8] = (seq_t)3272;
        computed = 8;
    }

    if (n <= computed)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t term1 = cache[k - 1];
        seq_t term2 = cache[k - 2];
        seq_t term3 = (seq_t)13 * cache[k - 3];
        seq_t term4 = (seq_t)2 * cache[k - 4];
        seq_t term5 = cache[k - 5];
        seq_t term6 = (seq_t)4 * term5;
        seq_t term7 = (seq_t)11 * cache[k - 6];
        seq_t term8 = (seq_t)3 * cache[k - 7];
        seq_t term9 = cache[k - 8];

        /* a(k) = term1 + term2 + term3 + term6 - term4 - term7 - term8 - term9 */
        cache[k] = term1 + term2 + term3 + term6
                 - term4 - term7 - term8 - term9;
    }
    computed = n;
    return cache[n];
}

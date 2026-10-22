/*
 * A399185  a(n) is the number of tilings of a 5 X 2n rectangle into
 *          Z-tetrominoes and U-pentominoes.
 * https://oeis.org/A399185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 4*a(n-5) + a(n-6) for n > 6 a(0)=1,
 *            a(1)=a(2)=a(3)=a(4)=0, a(5)=2, a(6)=0
 * Range:     u8 n <= 23, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache of already computed terms */
static seq_t *cache = NULL;
static size_t cap = 0;     /* current allocation size (number of entries) */
static idx_t computed_max = 0; /* highest index that has been computed */

/* Make sure cache has room for indices up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cap)
        return;

    size_t newcap = cap > 0 ? cap : 16;
    while (newcap <= (size_t)n)
        newcap <<= 1;          /* double until we can hold n */

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp && newcap > 0) {
        /* Allocation failed – terminate (cannot continue safely) */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cap = newcap;
}

/* Recurrence implementation */
seq_t a(idx_t n)
{
    /* Ensure storage for index n */
    ensure_capacity(n);

    /* Initialize base values if not already done */
    if (computed_max < 6) {
        if (cap < 7)
            ensure_capacity(6);   /* make sure indices 0..6 fit */

        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache[5] = 2;
        cache[6] = 0;

        computed_max = 6;
    }

    /* If the desired term is already computed, return it */
    if (computed_max > n || (computed_max == n))
        return cache[n];

    /* Compute any missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 6) {
            /* Base cases are already set */
            continue;
        }
        seq_t v4 = cache[i - 4];
        seq_t v5 = cache[i - 5];
        seq_t v6 = cache[i - 6];
        cache[i] = v4 + (seq_t)4 * v5 + v6;
    }

    computed_max = n;
    return cache[n];
}

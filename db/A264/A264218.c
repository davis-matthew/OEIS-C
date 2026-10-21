/*
 * A264218  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change 0,0 0,1 1,0 -1,0 or
 *          0,-2.
 * https://oeis.org/A264218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 6*a(n-3) + 2*a(n-4) - 2*a(n-5) -
 *            a(n-6) * with initial values a(1)=4, a(2)=17, a(3)=56, a(4)=172,
 *            a(5)=561, a(6)=1826
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* array of computed terms, index 1.. */
static idx_t  capacity = 0;          /* size of allocated array (number of elems)  */
static idx_t  computed = 0;          /* highest index for which a[i] is known */

static const seq_t base[7] = {0,4,17,56,172,561,1826};

/* Ensure that cache holds at least indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (capacity >= n + 1)
        return;
    seq_t *tmp = seq_realloc(cache, capacity, n + 1);
    if (tmp == NULL)                /* allocation failure – aborting is fine */
        exit(1);
    cache = tmp;
    capacity = n + 1;
}

/* Compute a(n) using the recurrence.  The routine is memoized and
 * independent of call order.  It works for all n >= 1. */
seq_t a(idx_t n)
{
    if (n <= 6)
        return base[n];

    ensure_capacity(n);

    /* Initialise base values if they are not yet present. */
    if (computed < 6) {
        for (idx_t i = computed + 1; i <= 6; ++i)
            cache[i] = base[i];
        computed = 6;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pos = 2 * cache[i - 1] + 2 * cache[i - 2] +
                    6 * cache[i - 3] + 2 * cache[i - 4];
        seq_t neg = 2 * cache[i - 5] + cache[i - 6];
        cache[i]  = pos - neg;
    }
    computed = n;
    return cache[n];
}

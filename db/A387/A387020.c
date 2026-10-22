/*
 * A387020  Number of permutations (p(1),p(2),...,p(n)) of (1,2,...,n) such
 *          that p(i)-i is in {-2,0,5} for all i=1,...,n.
 * https://oeis.org/A387020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+3*a(n-7)-2*a(n-8)+2*a(n-9)-a(n-10)+a(n-11)
 *            -3*a(n-14)+a(n-15)-2*a(n-16)+a(n-21) for n ≥ 21 Initial values
 *            (0 ≤ n ≤ 20):
 *            1,1,1,1,1,1,1,2,3,5,7,10,13,16,20,25,34,46,67,94,130
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[21] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7, (seq_t)10, (seq_t)13, (seq_t)16,
    (seq_t)20, (seq_t)25, (seq_t)34, (seq_t)46, (seq_t)67, (seq_t)94,
    (seq_t)130
};

static seq_t *cache   = NULL;   /* memoised values */
static idx_t  cache_sz = 0;      /* number of entries stored */

/* Return a(n) for the sequence A387020. */
seq_t a(idx_t n)
{
    /* Grow cache if needed. */
    if (n + 1 > cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;

        /* Initialise the base values that were not yet set. */
        if (cache_sz < 21) {
            size_t start = cache_sz;
            size_t end   = new_sz < 21 ? new_sz : 21;
            for (size_t i = start; i < end; ++i)
                cache[i] = base_vals[i];
        }

        /* Compute terms from the first missing index up to n. */
        size_t start = cache_sz;
        if (start < 21) start = 21;   /* the recurrence applies for n ≥ 21 */
        for (size_t i = start; i <= (size_t)n; ++i) {
            /* Sum the positive contributions. */
            seq_t pos = cache[i - 1];
            pos += cache[i - 7] * 3;
            pos += cache[i - 9] * 2;
            pos += cache[i - 11];
            pos += cache[i - 15];
            pos += cache[i - 21];

            /* Sum the negative contributions. */
            seq_t neg = cache[i - 8] * 2;
            neg += cache[i - 10];
            neg += cache[i - 14] * 3;
            neg += cache[i - 16] * 2;

            /* pos ≥ neg for all n, so unsigned subtraction is safe. */
            cache[i] = pos - neg;
        }
        cache_sz = new_sz;
    }
    return cache[(size_t)n];
}

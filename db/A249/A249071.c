/*
 * A249071  a(n) = A004001(2*n) - n, where A004001 is Hofstadter-Conway $10000
 *          sequence.
 * https://oeis.org/A249071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A004001(2*n) - n where A004001(1)=1, A004001(2)=1, and
 *            for k>2: A004001(k) = A004001(A004001(k-1)) + A004001(k -
 *            A004001(k-1))
 * Range:     u8 n <= 1178, u16 n <= 9814, u32 n <= 9814, u64 n <= 9814,
 *            u128 n <= 9814, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for values of A004001 up to some index */
static seq_t *cache = NULL;
static idx_t computed_max = 0;

/* ensure that cache holds values up to 'need' (inclusive) */
static void ensure_cache(idx_t need)
{
    if (need <= computed_max)
        return;

    /* allocate or enlarge cache */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(need + 1) * sizeof(seq_t));
    if (new_cache == NULL)
        return;                         /* allocation failed – abort silently */
    cache = new_cache;

    /* initialise base values if this is the first run */
    if (computed_max == 0) {
        cache[0] = 0;                    /* unused, just for safety */
        cache[1] = 1;
        cache[2] = 1;
        computed_max = 2;
    }

    /* compute A004001 values from computed_max+1 up to need */
    for (idx_t i = computed_max + 1; i <= need; ++i) {
        idx_t a_prev = (idx_t)cache[i - 1];
        seq_t term1   = cache[a_prev];
        seq_t term2   = cache[i - a_prev];
        cache[i] = term1 + term2;
    }

    computed_max = need;
}

/* sequence a(n) as defined above, with offset 1 */
seq_t a(idx_t n)
{
    /* n >= 1, so 2*n >= 2 */
    idx_t m = 2 * n;
    ensure_cache(m);
    return cache[m] - (seq_t)n;
}

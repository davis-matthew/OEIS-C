/*
 * A241891  Total number of unit squares appearing in the Thue-Morse sequence
 *          of logical matrices (1, 0 version) after n stages.
 * https://oeis.org/A241891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+5*a(n-2)-20*a(n-3)-4*a(n-4)+16*a(n-5) * for n >=
 *            6, with initial terms * a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=20,
 *            a(5)=72
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *seq_cache = NULL;     /* dynamic array of already computed terms   */
static size_t seq_cap   = 0;        /* allocated capacity of seq_cache           */
static size_t seq_used  = 0;        /* number of terms that are currently stored */

static const seq_t init_vals[6] = {
    (seq_t)1, /* a(0) */
    (seq_t)2, /* a(1) */
    (seq_t)4, /* a(2) */
    (seq_t)8, /* a(3) */
    (seq_t)20,/* a(4) */
    (seq_t)72 /* a(5) */
};

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    if (n < 6u)
        return init_vals[(size_t)n];

    size_t target = (size_t)n;

    /* Ensure the cache can hold all terms up to target. */
    if (target + 1 > seq_cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Initialise the cache with the first six terms on the first call. */
    if (seq_used == 0) {
        size_t init_len = seq_cap < 6 ? seq_cap : 6;
        for (size_t i = 0; i < init_len; ++i)
            seq_cache[i] = init_vals[i];
        seq_used = init_len;
    }
    if (seq_used < 6) {              /* cover the unlikely case of a smaller resize */
        for (size_t i = seq_used; i < 6; ++i)
            seq_cache[i] = init_vals[i];
        seq_used = 6;
    }

    /* Compute all missing terms up to target. */
    for (size_t i = seq_used; i <= target; ++i) {
        seq_t term1 = seq_cache[i-1];
        seq_t term2 = seq_cache[i-2];
        seq_t term3 = seq_cache[i-3];
        seq_t term4 = seq_cache[i-4];
        seq_t term5 = seq_cache[i-5];

        /* Build the expression avoiding negative intermediate values. */
        seq_t big   = ( (seq_t)4 * term1 ) + ( (seq_t)5 * term2 ) + ( (seq_t)16 * term5 );
        seq_t small = ( (seq_t)20 * term3 ) + ( (seq_t)4 * term4 );
        seq_cache[i] = big - small;
    }
    seq_used = target + 1;
    return seq_cache[target];
}

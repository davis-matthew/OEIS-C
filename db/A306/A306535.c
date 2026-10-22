/*
 * A306535  Number of permutations p of [2n] having no index i with |p(i)-i| =
 *          n.
 * https://oeis.org/A306535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = derangement number D_{2n} (i.e. the number of
 *            permutations of 2n elements with no element fixed by the
 *            involution that swaps each i with i+n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *der_cache   = NULL;   /* cache of derangements D_k for k ≤ m */
static idx_t  der_cache_size = 0;    /* next free index in der_cache */
static idx_t  der_cache_cap  = 0;    /* allocated capacity of der_cache */

static void ensure_der_cache(idx_t m)
{
    /* initialise cache if it does not exist */
    if (der_cache == NULL) {
        der_cache_cap = 2;
        der_cache = (seq_t*)calloc(der_cache_cap, sizeof(seq_t));
        der_cache[0] = (seq_t)1;   /* D_0 = 1 */
        der_cache[1] = (seq_t)0;   /* D_1 = 0 */
        der_cache_size = 2;
    }

    /* enlarge capacity if necessary */
    if ((idx_t)(m + 1) > der_cache_cap) {
        idx_t new_cap = der_cache_cap;
        while ((idx_t)(m + 1) > new_cap)
            new_cap <<= 1;            /* double until large enough */
        der_cache = (seq_t*)realloc(der_cache, new_cap * sizeof(seq_t));
        der_cache_cap = new_cap;
    }

    /* compute missing derangement values */
    for (idx_t k = der_cache_size; k <= m; ++k) {
        /* D_k = (k-1) * (D_{k-1} + D_{k-2})   (recurrence for k >= 2) */
        seq_t sum = der_cache[k - 1] + der_cache[k - 2];
        der_cache[k] = ((seq_t)(k - 1)) * sum;
    }
    der_cache_size = m + 1;
}

seq_t a(idx_t n)
{
    idx_t m = n * ((idx_t)2);          /* we need D_{2n} */
    ensure_der_cache(m);
    return der_cache[m];
}

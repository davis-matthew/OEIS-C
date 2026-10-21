/*
 * A259877  If n is even then a(n) = n!/( 2^(n/2)*(n/2)! ), otherwise a(n) =
 *          n!/( 3*2^((n-1)/2)*((n-3)/2)! ).
 * https://oeis.org/A259877
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(2)=1, a(n+1)=a(n)*n*(n+1)/6  if n even,
   a(n+1)=6*a(n)/(n-1)                                 if n odd. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cache[0] holds a(2), cache[1] holds a(3), … */
static idx_t  cache_size = 0;    /* allocated slots in cache               */
static idx_t  computed   = 0;    /* number of terms already computed: a(2) … a(2+computed-1) */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2. For n < 2 we return 0. */
    if (n < 2) return (seq_t)0;

    /* We need to store terms a(2) … a(n), i.e. (n‑1) values. */
    idx_t need = n - 1;

    /* Allocate or grow the cache if necessary. */
    if (cache_size < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_size = need;
    }

    /* Initialise the cache if this is the first call. */
    if (computed == 0) {
        cache[0] = (seq_t)1;   /* a(2) = 1 */
        computed = 1;
    }

    /* Compute missing terms up to a(n) using the recurrence. */
    while (computed < need) {
        /* Index of the previous term a(prev_n). */
        idx_t prev_n = 2 + computed - 1;  /* prev_n = 2 + (computed-1) */
        seq_t prev = cache[computed - 1];
        seq_t cur;

        if ((prev_n & 1) == 0) {          /* prev_n even → recurrence for even */
            cur = ((seq_t)prev * (seq_t)prev_n * (seq_t)(prev_n + 1)) / 6;
        } else {                          /* prev_n odd  → recurrence for odd  */
            cur = ((seq_t)6 * prev) / ((seq_t)(prev_n - 1));
        }

        cache[computed] = cur;
        ++computed;
    }

    return cache[n - 2];   /* a(n) is stored at offset n‑2 */
}

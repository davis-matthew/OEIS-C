/*
 * A274955  Number of nX4 0..2 arrays with no element equal to any value at
 *          offset (-1,-1) (-2,0) or (0,-2) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A274955
    The sequence satisfies the linear recurrence
      a(n) = 6*a(n-1) + 4*a(n-3) + 13*a(n-4)
           - 74*a(n-5) - 8*a(n-6) - 4*a(n-8) + 8*a(n-9)   for n > 11
    with initial values
      a(1)=6,  a(2)=72,  a(3)=384,  a(4)=2376,  a(5)=14406,
      a(6)=87780, a(7)=536406, a(8)=3278016, a(9)=20027574,
      a(10)=122367012, a(11)=747675414.
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;     /* largest index currently stored */

static const seq_t base_vals[11] = {
    (seq_t)6,             /* a(1) */
    (seq_t)72,            /* a(2) */
    (seq_t)384,           /* a(3) */
    (seq_t)2376,          /* a(4) */
    (seq_t)14406,         /* a(5) */
    (seq_t)87780,         /* a(6) */
    (seq_t)536406,        /* a(7) */
    (seq_t)3278016,       /* a(8) */
    (seq_t)20027574,      /* a(9) */
    (seq_t)122367012,     /* a(10) */
    (seq_t)747675414      /* a(11) */
};

static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* allocate or grow cache; cast result for C++ */
    size_t new_size = (size_t)(n + 1);    /* index 0 unused */
    if (cache == NULL) {
        cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    } else {
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    }
    if (cache == NULL)
        abort();                          /* out of memory */

    /* store initial values */
    for (idx_t i = 1; i <= 11 && i <= n; ++i)
        cache[i] = base_vals[i - 1];

    /* compute recurrence for n >= 12 */
    for (idx_t i = 12; i <= n; ++i) {
        seq_t val = ((seq_t)6) * cache[i - 1];
        val += ((seq_t)4) * cache[i - 3];
        val += ((seq_t)13) * cache[i - 4];
        val -= ((seq_t)74) * cache[i - 5];
        val -= ((seq_t)8)  * cache[i - 6];
        val -= ((seq_t)4)  * cache[i - 8];
        val += ((seq_t)8)  * cache[i - 9];
        cache[i] = val;
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        abort();          /* sequence defined only for n >= 1 */
    ensure_cache(n);
    return cache[n];
}

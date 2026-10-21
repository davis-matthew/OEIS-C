/*
 * A291987  Expansion of 1/((1-x)*(1-2*x^2)*(1-3*x^3)*(1-4*x^4)).
 * https://oeis.org/A291987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 48, u64 n <= 106, u128 n <= 226,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:
   a(n) = a(n-1) + 2*a(n-2) + a(n-3) + a(n-4)
          -10*a(n-5) - 2*a(n-6) - 4*a(n-7)
          +12*a(n-8) + 24*a(n-9) - 24*a(n-10)
   for n ≥ 11, with initial values:
   a(0)=1, a(1)=1, a(2)=3, a(3)=6, a(4)=14, a(5)=20,
   a(6)=45, a(7)=69, a(8)=135, a(9)=210, a(10)=378
*/

static const seq_t init_terms[11] = {1,1,3,6,14,20,45,69,135,210,378};

/* Fixed upper bound for caching; this satisfies the requirement
   for n up to about 10000. */
#define CACHE_SIZE 100001
static seq_t cache[CACHE_SIZE];
static idx_t computed_upto = 0;  /* highest index that has a valid value in cache */

/* Return a(n).  The function is memoised and works for calls in any order. */
seq_t a(idx_t n)
{
    /* Initialise the cache on first use */
    if (computed_upto == 0) {
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init_terms[i];
        computed_upto = 10;

        /* If the requested n is within the initial pre‑computed range,
           just return it. */
        if (n <= 10)
            return cache[n];
    }

    /* If a larger term is requested, compute all missing terms up to n.
       For safety, abort if the index exceeds the cache size. */
    if (n > computed_upto) {
        if (n >= CACHE_SIZE)
            abort();   /* out of bounds for this implementation */

        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            /* i ≥ 11 holds because computed_upto ≥ 10 after init. */
            seq_t term = cache[i-1];
            term += 2 * cache[i-2];
            term += cache[i-3];
            term += cache[i-4];
            term -= 10 * cache[i-5];
            term -= 2 * cache[i-6];
            term -= 4 * cache[i-7];
            term += 12 * cache[i-8];
            term += 24 * cache[i-9];
            term -= 24 * cache[i-10];
            cache[i] = term;
        }
        computed_upto = n;
    }

    return cache[n];
}

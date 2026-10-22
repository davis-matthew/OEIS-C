/*
 * A315598  Coordination sequence Gal.3.57.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-7) - a(n-8) + a(n-9) - a(n-10)
 * initial values:
 *   a(0)=1, a(1)=6, a(2)=12, a(3)=12, a(4)=24, a(5)=30,
 *   a(6)=36, a(7)=36, a(8)=42, a(9)=54, a(10)=54.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Initial values for n = 0 .. 10 */
    if (cap == 0) {
        cap = 128;            /* reasonable starting capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)24;
        cache[5] = (seq_t)30;
        cache[6] = (seq_t)36;
        cache[7] = (seq_t)36;
        cache[8] = (seq_t)42;
        cache[9] = (seq_t)54;
        cache[10] = (seq_t)54;
        len = 11;   /* valid indices: 0 .. len-1 */
    }

    /* Return immediately if already computed */
    if (n < len) {
        return cache[n];
    }

    /* Grow the cache buffer if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* unrecoverable allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute values iteratively up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 1];
        val -= cache[i - 2];
        val += cache[i - 3];
        val += cache[i - 7];
        val -= cache[i - 8];
        val += cache[i - 9];
        val -= cache[i - 10];
        cache[i] = val;
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}

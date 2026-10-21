/*
 * A252070  Number of (n+2)X(3+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 3 or 4.
 * https://oeis.org/A252070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   empirical recurrence a(n) = 2*a(n-1) + 11*a(n-3) + 20*a(n-4) +
 *            12*a(n-5) - 7*a(n-6) - 69*a(n-7) - 49*a(n-8) + 69*a(n-9) +
 *            59*a(n-10) - 9*a(n-11) - 30*a(n-12) - 14*a(n-13) + 2*a(n-14) +
 *            4*a(n-16) for n > 18 Initial values: a(1)=183, a(2)=419,
 *            a(3)=1497, a(4)=5085, a(5)=17455, a(6)=60245, a(7)=2067...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static const seq_t init_val[23] = {
    (seq_t)0ULL,           /* unused 0‑th element */
    (seq_t)183ULL, (seq_t)419ULL, (seq_t)1497ULL,
    (seq_t)5085ULL, (seq_t)17455ULL, (seq_t)60245ULL,
    (seq_t)206747ULL, (seq_t)711749ULL, (seq_t)2455039ULL,
    (seq_t)8451165ULL, (seq_t)29078125ULL, (seq_t)100127009ULL,
    (seq_t)344830169ULL, (seq_t)1187280673ULL, (seq_t)4087598979ULL,
    (seq_t)14073866379ULL, (seq_t)48459148697ULL,
    (seq_t)166851565147ULL, (seq_t)574482172193ULL,
    (seq_t)1977991833479ULL, (seq_t)6810453196661ULL,
    (seq_t)23449166058617ULL
};

seq_t a(idx_t n)
{
    if (n <= 22)
        return init_val[(idx_t)n];

    /* static cache for memoised values */
    static seq_t *cache     = NULL;   /* array of seq_t elements */
    static size_t cap       = 0;      /* allocated size */
    static idx_t  cached_max = 0;     /* highest index computed */

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache = seq_alloc_new_array(23);
        for (idx_t i = 0; i < 23; ++i)
            cache[i] = init_val[i];
        cap = 23;
        cached_max = 22;
    }

    /* grow cache if required */
    if ((size_t)n >= cap) {
        seq_t *new_cache = seq_alloc_new_array((size_t)n + 1);
        for (idx_t i = 0; i <= cached_max; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        cap   = (size_t)n + 1;
    }

    /* compute any missing terms up to n using the recurrence */
    if (cached_max < n) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t term = (seq_t)2 * cache[i-1];
            term += (seq_t)11 * cache[i-3];
            term += (seq_t)20 * cache[i-4];
            term += (seq_t)12 * cache[i-5];
            term -= (seq_t)7  * cache[i-6];
            term -= (seq_t)69 * cache[i-7];
            term -= (seq_t)49 * cache[i-8];
            term += (seq_t)69 * cache[i-9];
            term += (seq_t)59 * cache[i-10];
            term -= (seq_t)9  * cache[i-11];
            term -= (seq_t)30 * cache[i-12];
            term -= (seq_t)14 * cache[i-13];
            term += (seq_t)2  * cache[i-14];
            term += (seq_t)4  * cache[i-16];
            cache[i] = term;
        }
        cached_max = n;
    }

    return cache[n];
}

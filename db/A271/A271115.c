/*
 * A271115  Numbers whose square is a chiliagonal (or 1000-gonal) number.
 * https://oeis.org/A271115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 80640398*a(n-3)-a(n-6)  for n>6,
 * a(1)=1, a(2)=50049, a(3)=410924801, a(4)=491565199,
 * a(5)=4035971329551, a(6)=33137139500710799
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 holds a(1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;       /* allocated size */
    static size_t cache_size = 0;      /* number of terms cached */

    if (n == 0) return 0;              /* undefined for 0, return 0 */

    /* if already cached, just return */
    if (cache_size >= (size_t)n)
        return cache[n-1];

    /* make sure we have enough room */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : (size_t)n;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initial values for n=1..6 */
    static const seq_t init_v[6] = {
        (seq_t)1,
        (seq_t)50049,
        (seq_t)410924801,
        (seq_t)491565199,
        (seq_t)4035971329551ULL,
        (seq_t)33137139500710799ULL
    };

    /* compute terms from cached size up to n */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        size_t idx = i + 1;                /* 1‑based index */
        if (idx <= 6) {
            cache[i] = init_v[idx-1];
        } else {
            /* recurrence: a(idx) = 80640398*a(idx-3) - a(idx-6) */
            cache[i] = (seq_t)80640398 * cache[idx-4] - cache[idx-7];
        }
    }

    cache_size = (size_t)n;
    return cache[n-1];
}

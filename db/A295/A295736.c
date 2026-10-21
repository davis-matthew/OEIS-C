/*
 * A295736  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 1,
 *          a(1) = -2, a(2) = -2, a(3) = 1.
 * https://oeis.org/A295736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) with a(0)=1,
 *            a(1)=-2, a(2)=-2, a(3)=1
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#define ALLOC(p, n) (p) = reinterpret_cast<seq_t*>(calloc(n, sizeof(seq_t)));
#define REALLOC(p, n) (p) = reinterpret_cast<seq_t*>(realloc(p, (n) * sizeof(seq_t)));
#else
#define ALLOC(p, n) (p) = (seq_t*)calloc(n, sizeof(seq_t));
#define REALLOC(p, n) (p) = (seq_t*)realloc(p, (n) * sizeof(seq_t));
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t sz = 0;      /* number of terms actually computed */
    static size_t cap = 0;     /* allocated capacity */

    /* initialisation */
    if (cache == NULL) {
        ALLOC(cache, 4);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t)1;
        sz = 4;
        cap = 4;
    }

    /* already known? */
    if ((size_t)n < sz) {
        return cache[(size_t)n];
    }

    /* ensure enough room */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap > 0 ? cap * 2 : 4;
        while (new_cap < needed)
            new_cap *= 2;
        REALLOC(cache, new_cap);
        if (!cache) abort();
        cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        cache[i] = a1 + (seq_t)3 * a2 - (seq_t)2 * a3 - (seq_t)2 * a4;
    }
    sz = (size_t)n + 1;

    return cache[(size_t)n];
}

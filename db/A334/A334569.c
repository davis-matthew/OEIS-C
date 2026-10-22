/*
 * A334569  E.g.f.: exp(-(x + x^2/2 + x^3/3)).
 * https://oeis.org/A334569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 16, i64 n <= 27, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334569:
   EGF: exp(-(x + x^2/2 + x^3/3))
   Recurrence: a(0)=1, a(1)=-1, a(2)=0,
   a(n) = -a(n-1) - (n-1)*a(n-2) - (n-1)*(n-2)*a(n-3)  for n>=3
   This implements the D‑finite recurrence
   a(n)+a(n-1)+(n-1)a(n-2)+(n-1)(n-2)a(n-3)=0   (n>=3)   */

#include <stdlib.h>
#include <stdint.h>

/* static memoisation storage */
static seq_t *cache = NULL;   /* pointers to a(0),a(1),… */
static idx_t cache_len = 0;   /* number of values currently cached (max index+1) */

/* compute values up to n and cache them */
static void ensure_up_to(idx_t n)
{
    if (cache_len > n)
        return;   /* already computed */

    /* grow the cache array size to at least n+1 */
    idx_t new_size = cache_len ? (cache_len << 1) : 4;
    while (new_size <= n)
        new_size <<= 1;

    size_t new_size_t = (size_t)new_size;
    seq_t *tmp;
#if defined(__cplusplus)
    tmp = reinterpret_cast<seq_t*>(realloc(cache, new_size_t * sizeof(seq_t)));
#else
    tmp = (seq_t*)realloc(cache, new_size_t * sizeof(seq_t));
#endif
    if (!tmp)
        abort();   /* allocation failure */

    cache = tmp;

    /* fill in new entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)-1;
        } else if (i == 2) {
            cache[2] = (seq_t)0;
        } else {
            /* a(n) = -a(n-1) - (n-1)*a(n-2) - (n-1)*(n-2)*a(n-3) */
            seq_t t = -cache[i - 1];
            t -= (seq_t)(i - 1) * cache[i - 2];
            t -= (seq_t)(i - 1) * (i - 2) * cache[i - 3];
            cache[i] = t;
        }
    }
    cache_len = n + 1;
}

/* required public function */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}

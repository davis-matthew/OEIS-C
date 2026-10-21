/*
 * A270656  a(3n)=a(n), a(3n+1)=a(3n)-a(3(n+1)), a(3n+2)=a(3n)+a(3(n+1));
 *          a(0)=2, a(1)=1.
 * https://oeis.org/A270656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 9575, i16 n <= 9575, i32 n <= 9575, i64 n <= 9575,
 *            i128 n <= 9575, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 2, a(1) = 1
   a(3n)   = a(n)
   a(3n+1) = a(3n) - a(3(n+1))
   a(3n+2) = a(3n) + a(3(n+1))
   (Computed by memoised recursion)
*/
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

/* static cache */
static idx_t cache_cap = 0;                /* capacity (number of entries) */

#ifdef __cplusplus
/* C++ version: use std::vector to correctly handle non-trivial seq_t */
static std::vector<seq_t>      cache_vec;
static std::vector<unsigned char> computed_vec;

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_vec.size())
        return;
    size_t new_cap = cache_vec.empty() ? 16ull : (size_t)cache_vec.size() << 1;
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;
    cache_vec.resize(new_cap);
    computed_vec.resize(new_cap);    /* new elements default to 0 */
}

static seq_t* cache_ptr(idx_t)  { return cache_vec.data(); }
static unsigned char* computed_ptr(idx_t) { return computed_vec.data(); }
#else
/* C version: use malloc/realloc (seq_t must be trivially copyable) */
static seq_t  *cache      = NULL;
static unsigned char *computed = NULL;

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (size_t)cache_cap << 1 : 16ull;
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;
    seq_t *new_cache  = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *new_comp = (unsigned char*)realloc(computed, new_cap);
    if (new_cache == NULL || new_comp == NULL)
        return;                     /* allocation failure: leave cache unchanged */
    cache = new_cache;
    computed = new_comp;
    for (size_t i = cache_cap; i < new_cap; ++i)
        computed[i] = 0;
    cache_cap = new_cap;
}

static seq_t* cache_ptr(idx_t)  { return cache; }
static unsigned char* computed_ptr(idx_t) { return computed; }
#endif

/* Memoised recursive computation */
seq_t a(idx_t n)
{
    ensure_cache(n);
    seq_t  *c = cache_ptr(n);
    unsigned char *comp = computed_ptr(n);
    if (comp[n])
        return c[n];

    seq_t val;
    if (n == 0)
        val = (seq_t)2;
    else if (n == 1)
        val = (seq_t)1;
    else {
        idx_t r = n % 3;
        idx_t j = n / 3;
        if (r == 0) {
            val = a(j);
        } else {
            seq_t a3j  = a(3 * j);
            seq_t a3jp = a(3 * (j + 1));
            if (r == 1)
                val = a3j - a3jp;
            else /* r == 2 */
                val = a3j + a3jp;
        }
    }
    c[n] = val;
    comp[n] = 1;
    return val;
}

/*
 * A271949  Somos's sequence {b(4,n)} defined in comment in A078495:
 *          a(0)=a(1)=...=a(10)=1; for n>=11,
 *          a(n)=(a(n-1)*a(n-10)+a(n-5)*a(n-6))/a(n-11).
 * https://oeis.org/A271949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)*a(n-10) + a(n-5)*a(n-6)) / a(n-11) with a(0) =
 *            a(1) = … = a(10) = 1
 * Range:     u8 n <= 21, u16 n <= 28, u32 n <= 38, u64 n <= 51, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache     = NULL;   /* cached values of the sequence   */
static idx_t  cache_cap = 0;     /* current capacity of the cache   */
static idx_t  computed  = 10;     /* greatest n that has already been computed */

/* Expand the cache so that it can hold indices 0 … n.
   Initialise the first 11 values (if they are within the new range). */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap > n) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : (idx_t)1;
    while (new_cap <= n) new_cap <<= 1;
    if (new_cap < (idx_t)1) new_cap = (idx_t)1; /* protect against overflow */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();

    /* Copy old data. */
    for (idx_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];

    /* Initialise the first 11 terms to 1 (if within bounds). */
    for (idx_t i = 0; i <= 10 && i < new_cap; ++i) new_cache[i] = (seq_t)1;

    free(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* Computation of a single term (uses __int128 for 64‑bit built‑ins).   */
#ifdef __cplusplus
#include <type_traits>

/* For integral seq_t types. */
template<typename T, bool Integral = std::is_integral<T>::value>
struct TermCalc
{
    static T compute(idx_t i, T *a)
    {
        unsigned __int128 p1 = (unsigned __int128)a[i-1] * a[i-10];
        unsigned __int128 p2 = (unsigned __int128)a[i-5] * a[i-6];
        unsigned __int128 num = p1 + p2;
        return (T)(num / a[i-11]);
    }
};

/* For arbitrary‑precision seq_t types. */
template<typename T>
struct TermCalc<T, false>
{
    static T compute(idx_t i, T *a)
    {
        return (a[i-1] * a[i-10] + a[i-5] * a[i-6]) / a[i-11];
    }
};
#else
/* Plain C: seq_t is assumed to be an unsigned integer type. */
static inline seq_t compute_term_c(idx_t i, seq_t *a)
{
    unsigned __int128 p1 = (unsigned __int128)a[i-1] * a[i-10];
    unsigned __int128 p2 = (unsigned __int128)a[i-5] * a[i-6];
    unsigned __int128 num = p1 + p2;
    return (seq_t)(num / a[i-11]);
}
#endif
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n <= 10) return (seq_t)1;          /* initial offset values    */

    ensure_cache_size(n);

    if (n <= computed)                      /* already known            */
        return cache[n];

    /* Compute all new terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
#ifdef __cplusplus
        cache[i] = TermCalc<seq_t>::compute(i, cache);
#else
        cache[i] = compute_term_c(i, cache);
#endif
    }
    computed = n;
    return cache[n];
}

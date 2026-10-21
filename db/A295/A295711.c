/*
 * A295711  Number of nX3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1, 2 or 4 1s.
 * https://oeis.org/A295711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 8*a(n-2) + 22*a(n-3) + 18*a(n-4) - 2*a(n-5)
 *            -21*a(n-6) -20*a(n-7) -5*a(n-8) +3*a(n-9) +11*a(n-10) +a(n-11)
 *            -2*a(n-12) Initial values:
 *            a(1)=8,a(2)=57,a(3)=385,a(4)=2580,a(5)=17364,a(6)=116902,
 *            a(7)=786804,a(8)=5295555,a(9)=35642190,a(10)=239892457,
 *            a(11)=1614612772,a(12)=10867264744
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <new>
#include <cstdlib>
#else
#include <stdlib.h>
#endif

static seq_t *a_cache = NULL;      /* array of cached terms (index 0 unused) */
static idx_t  a_last  = 0;         /* largest index that has been computed */
static size_t a_cap   = 0;         /* allocated size of a_cache */

/* Grow the cache to at least n+1 elements (index n may be used). */
static void a_grow(idx_t n)
{
    if ((size_t)n + 1 <= a_cap)
        return;

    size_t newcap = (size_t)n + 1;
    seq_t *newp = NULL;

#ifdef __cplusplus
    /* Allocate raw memory and construct each element with default ctor. */
    newp = static_cast<seq_t*>(operator new[](newcap * sizeof(seq_t), std::nothrow));
    if (!newp) std::abort();
    for (size_t i = 0; i < newcap; ++i)
        new (newp + i) seq_t();          /* placement new */

    /* Copy existing values to new location */
    if (a_cache) {
        for (size_t i = 0; i < a_cap; ++i)
            newp[i] = a_cache[i];
    }

    /* Destroy old objects and free memory */
    if (a_cache) {
        for (size_t i = 0; i < a_cap; ++i)
            a_cache[i].~seq_t();
        operator delete[](a_cache);
    }
#else
    newp = (seq_t*)realloc(a_cache, newcap * sizeof(seq_t));
    if (!newp) abort();
#endif

    a_cache = newp;
    a_cap   = newcap;
}

/* Initialise the first twelve terms (once only). */
static void a_init_first12(void)
{
    if (a_last >= 12)
        return;

    static const seq_t init[13] = {
        (seq_t)0,            /* placeholder for index 0 */
        (seq_t)8,  (seq_t)57,  (seq_t)385,
        (seq_t)2580, (seq_t)17364,
        (seq_t)116902, (seq_t)786804,
        (seq_t)5295555, (seq_t)35642190,
        (seq_t)239892457, (seq_t)1614612772,
        (seq_t)10867264744
    };

    a_grow(12);
    for (idx_t i = 1; i <= 12; ++i)
        a_cache[i] = init[i];
    a_last = 12;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset 1 – not used for computation */

    a_init_first12();

    if (n <= a_last)
        return a_cache[n];

    a_grow(n);

    /* Compute terms from a_last+1 up to n using the empirical recurrence. */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t val =
              (seq_t)5  * a_cache[i-1]
            + (seq_t)8  * a_cache[i-2]
            + (seq_t)22 * a_cache[i-3]
            + (seq_t)18 * a_cache[i-4]
            - (seq_t)2  * a_cache[i-5]
            - (seq_t)21 * a_cache[i-6]
            - (seq_t)20 * a_cache[i-7]
            - (seq_t)5  * a_cache[i-8]
            + (seq_t)3  * a_cache[i-9]
            + (seq_t)11 * a_cache[i-10]
            + (seq_t)1  * a_cache[i-11]
            - (seq_t)2  * a_cache[i-12];
        a_cache[i] = val;
    }

    a_last = n;
    return a_cache[n];
}

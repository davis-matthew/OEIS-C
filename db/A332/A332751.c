/*
 * A332751  The number of flips to go from Hamiltonian cycle beta_n to gamma_n
 *          in the Cameron graph of size n using Thomason's algorithm.
 * https://oeis.org/A332751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=6, a(2)=28, a(3)=108, a(4)=400, a(5)=1486, a(6)=5516; a(n)
 *            = 4*a(n-1) - a(n-2) - a(n-4) - a(n-6) for n > 6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for already computed values.  Index 0 is unused. */
static seq_t *cache = NULL;
static size_t  cap   = 0;   /* allocated size of cache */

/* Initial values stored as unsigned long long to avoid
   overflow at compile time for small seq_t types. */
static const unsigned long long init_vals_ul[7] = {
    0ULL, 6ULL, 28ULL, 108ULL, 400ULL, 1486ULL, 5516ULL
};

/* Ensure that cache[0..n] exists; expand if necessary. */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cap)
        return;

    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < required)
        new_cap = required;

    seq_t *tmp = cache
                 ? (seq_t *)realloc(cache, new_cap * sizeof(seq_t))
                 : (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* allocation failed */

    /* Zero any newly added slots */
    for (size_t i = cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    cache = tmp;
    cap   = new_cap;
}

/* Return a(n) for any n >= 1.  Calls may come in any order. */
seq_t a(idx_t n)
{
    if (n == 0)          /* outside the offset – not used but defined */
        return (seq_t)0;

    ensure_capacity(n);

    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {                 /* not computed yet */
            if (i <= 6) {
                cache[i] = (seq_t)init_vals_ul[i];
            } else {
                seq_t val = cache[i-1];
                val *= (seq_t)4;
                val -= cache[i-2];
                val -= cache[i-4];
                val -= cache[i-6];
                cache[i] = val;
            }
        }
    }
    return cache[n];
}

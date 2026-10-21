/*
 * A255799  Number of (n+2) X (6+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 1 and no antidiagonal sum 2 and no row sum 0 and no
 *          column sum 3.
 * https://oeis.org/A255799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 55, u32 n <= 151, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequences A255799.
 * Empirical recurrence: a(n) = 3*a(n-6) - 2*a(n-12) for n > 15
 * with the following initial values (offset 1):
 *   a(1)=1318, a(2)=472, a(3)=258, a(4)=263, a(5)=290, a(6)=214,
 *   a(7)=270, a(8)=358, a(9)=322, a(10)=419, a(11)=486, a(12)=334,
 *   a(13)=434, a(14)=622, a(15)=550, a(16)=731, a(17)=878, a(18)=574,
 *   a(19)=762, a(20)=1150, a(21)=1006, a(22)=1355, a(23)=1662, a(24)=1054.
 *
 * The function below computes any term a(n) (n ≥ 1) in O(n) time with
 * memoisation.  It works for arbitrary‑precision seq_t as well as for
 * fixed‑width unsigned integer types, provided the build supplies seq_t
 * and idx_t.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static initial values (1‑based, index 0 unused).  The literals are
 * cast to seq_t to avoid implicit type promotion problems.
 */
static const seq_t init_values[25] = {
    0,          /* dummy for index 0 */
    (seq_t)1318ULL, (seq_t)472ULL,  (seq_t)258ULL,  (seq_t)263ULL,
    (seq_t)290ULL, (seq_t)214ULL,  (seq_t)270ULL,  (seq_t)358ULL,
    (seq_t)322ULL, (seq_t)419ULL,  (seq_t)486ULL,  (seq_t)334ULL,
    (seq_t)434ULL, (seq_t)622ULL,  (seq_t)550ULL,  (seq_t)731ULL,
    (seq_t)878ULL, (seq_t)574ULL,  (seq_t)762ULL,  (seq_t)1150ULL,
    (seq_t)1006ULL,(seq_t)1355ULL, (seq_t)1662ULL, (seq_t)1054ULL
};
static const idx_t init_len = sizeof(init_values) / sizeof(init_values[0]);

seq_t a(idx_t n) {
    /* Lazy static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (maximum index + 1) */
    static bool initialized = false;

    if (n < 1) return (seq_t)0;   /* undefined for n = 0 or negative */

    /* Ensure the cache has enough space to contain all terms up to n,
       but always keep at least `init_len` elements so the initial values
       can be stored safely.
     */
    size_t required = n + 1;
    if (required < init_len) required = init_len;
    if (required > cache_cap) {
        seq_t *new_ptr = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_ptr) return (seq_t)0;          /* allocation failed */
        cache = new_ptr;
        cache_cap = required;
    }

    /* Initialise the first `init_len-1` entries only once. */
    if (!initialized) {
        for (idx_t i = 1; i < init_len; ++i)
            cache[i] = init_values[i];
        initialized = true;
    }

    /* If we already know the requested term, return it. */
    if (n < init_len) {
        return cache[n];
    }
    /* Compute any missing terms up to n using the empirical recurrence. */
    for (idx_t i = init_len; i <= n; ++i) {
        /* a(i) = 3*a(i-6) - 2*a(i-12)  */
        seq_t term = cache[i - 6];
        term = term * (seq_t)3;              /* 3*a(i-6) */
        term = term - cache[i - 12] * (seq_t)2; /* -2*a(i-12) */
        cache[i] = term;
    }

    return cache[n];
}

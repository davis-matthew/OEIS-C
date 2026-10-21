/*
 * A250062  Number of length 3+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) + 252*a(n-5)
 *              - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) - a(n-10)
 * Initial values:
 *   a(1) = 21,  a(2) = 2292,  a(3) = 50028,  a(4) = 505154,  a(5) = 3213429,
 *   a(6) = 15002490,  a(7) = 56118016,  a(8) = 177725772,  a(9) = 494492733,
 *   a(10) = 1240508808
 *
 * The function computes terms on demand.  It stores all computed terms
 * in a static dynamic array that is resized as necessary.  The code
 * uses only C/C++ standard facilities and works with any integer type
 * supplied for seq_t and idx_t (including arbitrary‑precision types
 * that overload the usual operators).
 */

#include <stdlib.h>   /* realloc, abort */

#if defined(__cplusplus)
#  define RECAST_SEQ_T_PTR(p) reinterpret_cast<seq_t*>(p)
#else
#  define RECAST_SEQ_T_PTR(p) ((seq_t*)(p))
#endif

seq_t a(idx_t n)
{
    /* initial values for n = 1 … 10 */
    static const seq_t init[10] = {
        21ULL, 2292ULL, 50028ULL, 505154ULL,
        3213429ULL, 15002490ULL, 56118016ULL,
        177725772ULL, 494492733ULL, 1240508808ULL
    };

    /* simple lookup for the first 10 terms */
    if (n <= 10)
        return init[n - 1];

    /* static state for memoisation */
    static seq_t *cache = NULL;   /* array of a(1) .. a(max_computed) */
    static size_t cap   = 0;      /* current allocated size of cache */
    static idx_t  computed = 0;   /* largest n that has been computed */

    /* enlarge cache if necessary */
    if ((size_t)n > cap) {
        void *tmp = realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();          /* cannot allocate */
        cache = RECAST_SEQ_T_PTR(tmp);
        cap   = (size_t)n;
    }

    /* initialise first ten terms if not yet done */
    if (computed < 10) {
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        computed = 10;
    }

    /* compute terms from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 2] * (seq_t)10;           /* a(i-1) */
        val -= cache[i - 3] * (seq_t)45;                /* a(i-2) */
        val += cache[i - 4] * (seq_t)120;               /* a(i-3) */
        val -= cache[i - 5] * (seq_t)210;               /* a(i-4) */
        val += cache[i - 6] * (seq_t)252;               /* a(i-5) */
        val -= cache[i - 7] * (seq_t)210;               /* a(i-6) */
        val += cache[i - 8] * (seq_t)120;               /* a(i-7) */
        val -= cache[i - 9] * (seq_t)45;                /* a(i-8) */
        val += cache[i -10] * (seq_t)10;                /* a(i-9) */
        val -= cache[i -11];                           /* a(i-10) */

        cache[i - 1] = val;
    }

    computed = n;
    return cache[n - 1];
}

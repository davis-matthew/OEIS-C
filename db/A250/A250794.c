/*
 * A250794  Number of (n+1) X (5+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250794: Number of (n+1)×6 binary arrays with the stated monotonicity
   constraints.  The sequence satisfies the 9‑term linear recurrence
   (OEIS) for n > 9:
      a(n) = 10*a(n-1) - 33*a(n-2) + 24*a(n-3) + 79*a(n-4)
            - 138*a(n-5) - 3*a(n-6) + 100*a(n-7) - 18*a(n-8) - 20*a(n-9)
   Initial values (n = 1 … 9):
      252, 838, 2776, 9258, 31220, 105954, 361344, 1236058, 4237556.
*/
#include <stddef.h>
#include <stdlib.h>

/* External integer types supplied by the build system */
extern seq_t a(idx_t n);

/* Pre‑computed first nine terms (index 0 unused).  The array is
   used only for the base cases; the dynamic 'cache' array holds all
   terms including the base ones. */
static const seq_t base_terms[10] = {
    (seq_t)0,          /* base_terms[0] unused */
    (seq_t)252,        /* a(1) */
    (seq_t)838,        /* a(2) */
    (seq_t)2776,       /* a(3) */
    (seq_t)9258,       /* a(4) */
    (seq_t)31220,      /* a(5) */
    (seq_t)105954,     /* a(6) */
    (seq_t)361344,     /* a(7) */
    (seq_t)1236058,    /* a(8) */
    (seq_t)4237556     /* a(9) */
};

seq_t a(idx_t n)
{
    /* Quick return for the known first nine values. */
    if (n <= 9)
        return base_terms[n];

    /* Persistent cache for computed terms.  The cache uses a C
       reallocation strategy that works with both C and C++. */
    static seq_t *cache   = NULL;   /* array holding all computed a(k) */
    static size_t cache_cap = 0;    /* allocated capacity in elements   */
    static idx_t  max_computed = 0; /* largest n that has been computed */

    /* Ensure the cache is large enough for indices up to n. */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (cache_cap == 0) ? ((size_t)n + 1) : cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure – terminate safely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise cache entries for the first nine terms if they have
       not yet been stored.  The loop starts at the next index after
       the last computed one and continues up to 9. */
    if (max_computed < 9) {
        for (idx_t i = max_computed + 1; i <= 9; ++i)
            cache[i] = base_terms[i];
    }

    /* Compute any missing terms up to n using the recurrence. */
    idx_t start = (max_computed >= 9) ? max_computed + 1 : 10;
    for (idx_t i = start; i <= n; ++i) {
        /* Hash the recurrence.  All intermediate operations are
           performed in seq_t, which must support the usual
           arithmetic operators.  The recurrence guarantees
           non‑negative results, so unsigned subtraction is safe. */
        seq_t val = cache[i-1] * (seq_t)10
                  + cache[i-3] * (seq_t)24
                  + cache[i-4] * (seq_t)79
                  + cache[i-7] * (seq_t)100;

        val -= cache[i-2] * (seq_t)33;
        val -= cache[i-5] * (seq_t)138;
        val -= cache[i-6] * (seq_t)3;
        val -= cache[i-8] * (seq_t)18;
        val -= cache[i-9] * (seq_t)20;

        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}

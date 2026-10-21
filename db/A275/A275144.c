/*
 * A275144  Number of 4 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 10*a(n-3) - 6*a(n-4) + a(n-5) for n
 *            > 9 * Initial values: a(1)=14, a(2)=216, a(3)=400, a(4)=1088,
 *            a(5)=2320, * a(6)=6464, a(7)=17872, a(8)=49792, a(9)=139664 * *
 *            This implementation memoises the recurrence up to the largest n
 *            * requested, using a static array that is...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Static cache for memoisation.  Element 0 is unused to keep 1‑based indexing. */
static seq_t cache[10001 + 1] = {0};
static idx_t computed_up_to = 0;   /* highest index for which cache[i] is valid */

/* Pre‑initialised first 9 terms of the sequence */
static const seq_t init_terms[10] = {
    0ULL,   /* placeholder for index 0 */
    14ULL,
    216ULL,
    400ULL,
    1088ULL,
    2320ULL,
    6464ULL,
    17872ULL,
    49792ULL,
    139664ULL
};

seq_t a(idx_t n)
{
    /* n is expected to be >= 1; behaviour for n < 1 is undefined */
    if (n < (idx_t)1) return (seq_t)0;

    /* Ensure the cache contains the requested term */
    if ((idx_t)computed_up_to < n) {
        /* Loop from the next uncomputed index up to n */
        size_t k;
        for (k = (size_t)(computed_up_to + 1); k <= (size_t)n; ++k) {
            if (k <= 9) {
                /* Directly use the initial known terms */
                cache[k] = init_terms[k];
            } else {
                /* Apply the recurrence relation.  All operations are on seq_t. */
                seq_t term1 = 5ULL * cache[k - 1];
                seq_t term2 = 9ULL * cache[k - 2];
                seq_t term3 = 10ULL * cache[k - 3];
                seq_t term4 = 6ULL * cache[k - 4];
                seq_t term5 = cache[k - 5];

                cache[k] = term1 - term2 + term3 - term4 + term5;
            }
        }
        computed_up_to = (idx_t)k - 1;
    }

    return cache[n];
}

/*
 * A294546  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + n, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binary_in_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+b(n-1)+n
   Initial values: a(0)=1, a(1)=2, b(0)=3
   (a(n)) and (b(n)) are increasing complementary sequences.
   The implementation memoises the two sequences and lazily
   extends them.  For each new term a(n) we first find the
   smallest integer >b(n-2) that does not already appear
   in a[0..n-1] using binary search, then compute a(n).
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/binary_in_array.h"

/* Dynamic arrays for a and b */
static seq_t *a_seq = NULL;
static seq_t *b_seq = NULL;
static idx_t cache_cap = 0;   /* allocated size (>=2)   */
static idx_t cache_last = 0;  /* largest index that has
                                 been computed (>=1 after init) */

/* Grow the arrays to at least N+1 (or 2 if N<1) slots, preserving
   existing values.  Handles both C and C++ builds. */
static void grow_if_needed(idx_t N)
{
    size_t new_cap = (size_t)(N + 1);
    if (new_cap < 2)
        new_cap = 2;

    if (cache_cap >= new_cap)
        return;

    /* allocate new storage */
#if defined(__cplusplus)
    seq_t *new_a = new seq_t[new_cap];
    seq_t *new_b = new seq_t[new_cap];
#else
    seq_t *new_a = (seq_t *)calloc(new_cap, sizeof(seq_t));
    seq_t *new_b = (seq_t *)calloc(new_cap, sizeof(seq_t));
#endif

    /* copy old contents */
    if (a_seq) {
#if defined(__cplusplus)
        for (size_t i = 0; i < cache_cap; ++i)
            new_a[i] = a_seq[i];
        delete[] a_seq;
#else
        memcpy(new_a, a_seq, cache_cap * sizeof(seq_t));
        free(a_seq);
#endif
    }
    if (b_seq) {
#if defined(__cplusplus)
        for (size_t i = 0; i < cache_cap; ++i)
            new_b[i] = b_seq[i];
        delete[] b_seq;
#else
        memcpy(new_b, b_seq, cache_cap * sizeof(seq_t));
        free(b_seq);
#endif
    }

    a_seq = new_a;
    b_seq = new_b;
    cache_cap = new_cap;
}

/* Ensure that a[0..N] and b[0..N-1] are computed.  */

static void ensure_terms(idx_t N)
{
    if (N <= cache_last)
        return;

    grow_if_needed(N);

    /* Initialisation on first call */
    if (cache_last == 0) {
        a_seq[0] = 1;
        a_seq[1] = 2;
        b_seq[0] = 3;
        cache_last = 1;   /* a[1] is the highest computed index now */
    }

    /* Extend up to N */
    for (idx_t i = cache_last + 1; i <= N; ++i) {
        /* Compute b[i-1] – smallest integer > b[i-2] not in a[0..i-1] */
        seq_t cand = b_seq[i - 2] + 1;
        while (binary_in_array(a_seq, (size_t)i, cand))
            ++cand;
        b_seq[i - 1] = cand;

        /* Compute a[i] */
        a_seq[i] = a_seq[i - 1] + a_seq[i - 2] + b_seq[i - 1] + (seq_t)i;
    }

    cache_last = N;
}

/*---  Public function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        ensure_terms(1);          /* guarantees a[0] and a[1] exist */
        return a_seq[(size_t)n];
    }
    ensure_terms(n);
    return a_seq[(size_t)n];
}

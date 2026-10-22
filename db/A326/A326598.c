/*
 * A326598  Sum of the largest parts of the partitions of n into 10 parts.
 * https://oeis.org/A326598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the largest parts of the partitions of n into 10
 *            parts
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#  include <new>
#endif

/* static tables for the number of partitions f[n][k] and the sum of largest
   parts L[n][k] (k from 0 to 10).  All indices are stored in a flat array
   of size (max_n+1)*11.                                                              */
static seq_t *F = NULL;
static seq_t *Larr = NULL;
static idx_t computed_up_to = 0;
/* maximum k is always 10 for this sequence */
static const idx_t KMAX = 10;

/* Allocate an array of seq_t of the given count. */
static inline seq_t *allocate_seq(size_t count)
{
#if defined(__cplusplus)
    return new (std::nothrow) seq_t[count];
#else
    return (seq_t *)malloc(count * sizeof(seq_t));
#endif
}

/* Free an array allocated by allocate_seq. */
static inline void free_seq(seq_t *ptr)
{
#if defined(__cplusplus)
    delete[] ptr;
#else
    free(ptr);
#endif
}

/* Reallocate an existing array to a new size, copying the old contents. */
static inline seq_t *reallocate_seq(seq_t *oldp, size_t old_count, size_t new_count)
{
#if defined(__cplusplus)
    seq_t *newp = new (std::nothrow) seq_t[new_count];
    if (!newp) return NULL;
    for (size_t i = 0; i < old_count; ++i) newp[i] = oldp[i];
    delete[] oldp;
    return newp;
#else
    return (seq_t *)realloc(oldp, new_count * sizeof(seq_t));
#endif
}

/* Initialise the tables for n = 0. */
static void init_dp(void)
{
    if (F != NULL) return;          /* already initialised */

    size_t sz = (size_t)1 * (KMAX + 1);  /* n = 0 row only */
    F     = allocate_seq(sz);
    Larr  = allocate_seq(sz);
    if (!F || !Larr) exit(EXIT_FAILURE);

    /* zero the entire row */
    for (size_t i = 0; i < sz; ++i) {
        F[i]    = (seq_t)0;
        Larr[i] = (seq_t)0;
    }

    /* base values */
    F[0 * (KMAX + 1) + 0]    = (seq_t)1;  /* f(0,0) = 1 */
    /* Larr[0][0] is already zero */

    computed_up_to = 0;
}

/* Extend the DP tables to cover all n <= limit. */
static void ensure_up_to(idx_t limit)
{
    init_dp();
    if (computed_up_to >= limit) return;

    idx_t old_limit = computed_up_to;
    computed_up_to = limit;

    size_t old_count = (size_t)(old_limit + 1) * (KMAX + 1);
    size_t new_count = (size_t)(limit + 1) * (KMAX + 1);

    seq_t *newF    = reallocate_seq(F,    old_count, new_count);
    seq_t *newLarr = reallocate_seq(Larr, old_count, new_count);
    if (!newF || !newLarr) exit(EXIT_FAILURE);
    F     = newF;
    Larr  = newLarr;

    /* zero the newly allocated cells */
    for (size_t i = old_count; i < new_count; ++i) {
        F[i]    = (seq_t)0;
        Larr[i] = (seq_t)0;
    }

    /* compute DP for each new n */
    for (idx_t n = old_limit + 1; n <= limit; ++n) {
        size_t base = (size_t)n * (KMAX + 1);

        /* f[n][0] = 0 for n > 0, already zero */
        Larr[base + 0] = (seq_t)0;

        idx_t maxk = (n < (idx_t)KMAX) ? n : (idx_t)KMAX;
        for (idx_t k = 1; k <= maxk; ++k) {
            if (k == 1) {
                /* only one partition of n into one part: [n] */
                F[base + 1]    = (seq_t)1;
                Larr[base + 1] = (seq_t)n;
            } else {
                /* f[n][k] = f[n-1][k-1] + f[n-k][k] */
                seq_t term1_f = F[(size_t)(n - 1) * (KMAX + 1) + (k - 1)];
                seq_t term2_f = (seq_t)0;
                if (n >= k) term2_f = F[(size_t)(n - k) * (KMAX + 1) + k];
                F[base + k] = term1_f + term2_f;

                /* L[n][k] = L[n-1][k-1] + L[n-k][k] + f[n-k][k] */
                seq_t term1_L = Larr[(size_t)(n - 1) * (KMAX + 1) + (k - 1)];
                seq_t term2_L = (seq_t)0;
                if (n >= k)
                    term2_L = Larr[(size_t)(n - k) * (KMAX + 1) + k] + F[(size_t)(n - k) * (KMAX + 1) + k];
                Larr[base + k] = term1_L + term2_L;
            }
        }
        /* indices k > n remain zero (already zeroed) */
    }
}

/* Public function as required.  Returns the sum of the largest part
   over all partitions of n into exactly 10 parts. */
seq_t a(idx_t n)
{
    if (n < 10) return (seq_t)0;          /* no partition into 10 parts */
    ensure_up_to(n);
    return Larr[(size_t)n * (KMAX + 1) + (idx_t)KMAX];
}

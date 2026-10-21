/*
 * A259994  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000011 or 00000101.
 * https://oeis.org/A259994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 50, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259994
 *
 * Recurrence:  a(n) = a(n-1) + 5*a(n-3) + 7*a(n-4)
 *                    - 7*a(n-5) - 5*a(n-7)
 * Initial terms:
 *   a(1)=48, a(2)=99, a(3)=284, a(4)=641,
 *   a(5)=1228, a(6)=2943, a(7)=7284
 *
 * The function memoises values in a static array so that
 * subsequent calls, even in non‑increasing order of n,
 * are fast.  Allocation uses malloc/realloc and the
 * constants are cast to seq_t so that the code compiles
 * both under C11 and C++17.
 */

#include <stdlib.h>
#include <string.h>

/* Forward declaration of the function requested by the interface */
seq_t a(idx_t n);

/* Static cache (index 0 unused).  The cache always holds
 * the computed values up to its current size.  It may be
 * reallocated to a larger size when more terms are needed.  */
static seq_t *seq_cache = NULL;      /* array of seq_t, 1‑based */
static idx_t  seq_cache_size = 0;    /* highest n that has been computed */

/* Initialise the cache with the first seven terms.  This
 * routine is idempotent. */
static void init_seq_cache(void)
{
    if (seq_cache != NULL)
        return;

    /* allocate space for indices 0…7 (index 0 unused) */
    seq_cache = (seq_t *)calloc(8, sizeof(seq_t));
    if (seq_cache == NULL) exit(1);   /* OOM -> terminate */
    seq_cache_size = 7;               /* a(7) is the largest pre‑computed value */

    seq_cache[1] = (seq_t)48;
    seq_cache[2] = (seq_t)99;
    seq_cache[3] = (seq_t)284;
    seq_cache[4] = (seq_t)641;
    seq_cache[5] = (seq_t)1228;
    seq_cache[6] = (seq_t)2943;
    seq_cache[7] = (seq_t)7284;
}

/* Ensure that the cache can store values up to n (inclusive).
 * If it already does, nothing is done.
 * The cache may be reallocated to a larger size;  all new
 * elements are zeroed to avoid garbage.  */
static void ensure_capacity(idx_t n)
{
    if (seq_cache == NULL)
        init_seq_cache();

    /* If we already have capacity up to n, nothing to do. */
    if (n <= seq_cache_size)
        return;

    /* Allocate a new array large enough for indices 0…n. */
    seq_t *new_cache = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL)
        exit(1);   /* OOM */

    /* Zero the elements that were added by the reallocation. */
    if (n + 1 > seq_cache_size + 1) {
        size_t added = (n + 1) - (seq_cache_size + 1);
        memset(new_cache + seq_cache_size + 1, 0,
               added * sizeof(seq_t));
    }

    seq_cache = new_cache;
    /* seq_cache_size will be updated after new terms are computed. */
}

/* Return the n‑th term of the sequence (n ≥ 1).
 * The function works for arbitrary order of calls, because
 * it keeps a memoised cache of the values that have already
 * been computed.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    if (seq_cache == NULL)
        init_seq_cache();

    if (n <= seq_cache_size)
        return seq_cache[n];

    /* Make sure the underlying array can hold up to n. */
    ensure_capacity(n);

    /* Compute terms from the next missing index up to n. */
    for (idx_t i = seq_cache_size + 1; i <= n; ++i) {
        /* The recurrence requires indices i‑1, i‑3, i‑4, i‑5, i‑7.
         * Those have all been computed because i ≥ 8. */
        seq_t term1 = seq_cache[i - 1];
        seq_t term2 = (seq_t)5 * seq_cache[i - 3];
        seq_t term3 = (seq_t)7 * seq_cache[i - 4];
        seq_t sum1  = term1 + term2 + term3;

        seq_t term4 = (seq_t)7 * seq_cache[i - 5];
        seq_t term5 = (seq_t)5 * seq_cache[i - 7];
        seq_t sum2  = term4 + term5;

        seq_cache[i] = sum1 - sum2;
    }

    /* All values up to n are now computed. */
    seq_cache_size = n;

    return seq_cache[n];
}

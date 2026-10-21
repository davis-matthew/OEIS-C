/*
 * A276926  Number of ordered set partitions of [n] with at most six elements
 *          per block.
 * https://oeis.org/A276926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=1..6} C(n,k) * a(n-k),   a(0) = 1
 *        (C(n,k) is the binomial coefficient "n choose k").
 *        This completes the recurrence derived from the e.g.f. 1/(1‑Σ_{i=1..6} x^i/i!).
 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *memo = NULL;      /* cache of computed values */
static size_t memo_cap = 0;     /* size of allocated cache */
static size_t memo_size = 0;    /* number of entries actually computed */

/* enlarge the cache so that it can hold at least n+1 entries */
static void ensure_cap(idx_t n)
{
    if ((size_t)n + 1 <= memo_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_mem = NULL;

#ifdef __cplusplus
    new_mem = new seq_t[new_cap];
#else
    new_mem = malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_mem) {
        /* Out‑of‑memory: abort the program (as no other error handling is required). */
        abort();
    }

    /* copy old data */
    for (size_t i = 0; i < memo_cap; ++i)
        new_mem[i] = memo[i];

    /* free old cache */
#ifdef __cplusplus
    delete[] memo;
#else
    free(memo);
#endif

    memo = new_mem;
    memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* make sure the cache can store a[0..n] */
    ensure_cap(n);

    /* base case: first time we run we need to initialise a[0]=1 */
    if (memo_size == 0) {
        memo[0] = (seq_t)1;
        memo_size = 1;
    }

    /* compute any missing values up to n */
    if (memo_size <= (size_t)n) {
        for (idx_t i = (idx_t)memo_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            /* sum over block sizes k = 1..6 (or up to i) */
            for (uint8_t k = 1; k <= 6 && (idx_t)k <= i; ++k) {
                /* compute binomial C(i,k) */
                seq_t bin = (seq_t)1;
                idx_t start = i - k + 1;
                for (uint8_t j = 1; j <= k; ++j) {
                    /* bin = bin * (start + j - 1) / j; */
                    bin = bin * (seq_t)(start + j - 1) / (seq_t)j;
                }
                /* add contribution to the sum */
                sum += bin * memo[i - k];
            }
            memo[i] = sum;
        }
        memo_size = (size_t)n + 1;
    }

    return memo[n];
}

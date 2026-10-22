/*
 * A350519  a(n) = A(n,n) where A(1,n) = A(n,1) = prime(n+1) and A(m,n) =
 *          A(m-1,n) + A(m,n-1) + A(m-1,n-1) for m > 1 and n > 1.
 * https://oeis.org/A350519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A(n,n) where A(1,n) = A(n,1) = prime(n+1) A(m,n) =
 *            A(m-1,n) + A(m,n-1) + A(m-1,n-1) (m>1, n>1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> diag;          /* diagonal values A(n,n) (1‑based) */
static idx_t max_n_cached = 0;          /* largest n for which diag is ready */

static void compute_to(idx_t n)
{
    if (n <= max_n_cached) return;      /* already done */

    if (n + 1 > diag.size()) diag.resize(n + 1);

    std::vector<seq_t> prev(n + 1, 0);
    std::vector<seq_t> curr(n + 1, 0);

    /* first row m = 1 */
    for (idx_t j = 1; j <= n; ++j)
        curr[(size_t)j] = (seq_t)get_prime((idx_t)j + 1);
    diag[1] = curr[1];

    /* rows m = 2 .. n */
    for (idx_t m = 2; m <= n; ++m)
    {
        std::swap(prev, curr);
        curr[1] = (seq_t)get_prime((idx_t)m + 1);
        for (idx_t j = 2; j <= n; ++j)
            curr[(size_t)j] = prev[(size_t)j] + curr[(size_t)(j - 1)]
                               + prev[(size_t)(j - 1)];
        diag[(size_t)m] = curr[m];
    }

    max_n_cached = n;
}
#else   /* C implementation */

/* C implementation uses malloc/free for arrays */
static seq_t *diag = NULL;                /* diagonal values A(n,n) (1‑based) */
static size_t diag_len = 0;               /* allocated size of diag */
static idx_t   max_n_cached = 0;          /* largest n that has been computed */

static void compute_to(idx_t n)
{
    if (n <= max_n_cached) return;        /* already done */

    /* grow diag array if necessary */
    if ((size_t)n + 1 > diag_len) {
        diag = (seq_t *)realloc(diag, (n + 1) * sizeof(seq_t));
        for (size_t i = diag_len; i <= (size_t)n; ++i)
            diag[i] = 0;
        diag_len = n + 1;
    }

    /* buffers for two consecutive rows */
    static seq_t *prev = NULL;
    static seq_t *curr = NULL;
    static size_t row_len = 0;

    if (row_len < (size_t)n + 1) {
        /* resize row buffers */
        prev = (seq_t *)realloc(prev, (n + 1) * sizeof(seq_t));
        curr = (seq_t *)realloc(curr, (n + 1) * sizeof(seq_t));
        for (size_t i = row_len; i <= (size_t)n; ++i) {
            prev[i] = 0;
            curr[i] = 0;
        }
        row_len = n + 1;
    }

    /* first row m = 1 */
    for (idx_t j = 1; j <= n; ++j)
        curr[(size_t)j] = (seq_t)get_prime((idx_t)j + 1);
    diag[1] = curr[1];

    /* rows m = 2 .. n */
    for (idx_t m = 2; m <= n; ++m) {
        /* swap buffers */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;

        curr[1] = (seq_t)get_prime((idx_t)m + 1);
        for (idx_t j = 2; j <= n; ++j)
            curr[(size_t)j] = prev[(size_t)j] + curr[(size_t)(j - 1)]
                               + prev[(size_t)(j - 1)];
        diag[(size_t)m] = curr[m];
    }

    max_n_cached = n;
}
#endif

/* required function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, harmless */
        return 0;

    compute_to(n);
    return diag[(size_t)n];
}

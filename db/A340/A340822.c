/*
 * A340822  a(n) = exp(-1) * Sum_{k>=0} (k*(k + n))^n / k!.
 * https://oeis.org/A340822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * Bell(2*n-k) * n^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses a cached table of Bell numbers up to 2*n. */

#include <stdlib.h>

static seq_t *bell_cache = NULL;   /* Cached Bell numbers B[0..max] */
static size_t bell_cap   = 0;      /* Allocated size of bell_cache      */
static size_t bell_max   = 0;      /* Highest index that has been
                                     computed and stored */

/* Ensure Bell numbers B[0..N] are available in bell_cache. */
static void ensure_bell_up_to(idx_t N)
{
    size_t needed = (size_t)N + 1;   /* number of elements needed */

    /* Allocate memory if this is the first call. */
    if (bell_cache == NULL) {
        bell_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (bell_cache == NULL) return;   /* allocation failed */
        bell_cap   = needed;
        bell_cache[0] = 1;              /* B0 = 1   */
        bell_max     = 0;
    }
    /* Grow the array if required. */
    else if (needed > bell_cap) {
        seq_t *tmp = (seq_t *)realloc(bell_cache, needed * sizeof(seq_t));
        if (tmp == NULL) return;        /* realloc failed */
        bell_cache = tmp;
        bell_cap   = needed;
    }

    /* Compute any Bell numbers that have not yet been filled. */
    if ((size_t)N <= bell_max) return;

    for (size_t m = bell_max + 1; m <= (size_t)N; ++m) {
        seq_t sum   = 0;
        seq_t binom = 1;          /* binomial coefficient C(m-1,0) */
        size_t r = m - 1;
        for (size_t k = 0; k <= r; ++k) {
            sum += binom * bell_cache[k];
            if (k < r)
                binom = (binom * (seq_t)(r - k)) / (seq_t)(k + 1);
        }
        bell_cache[m] = sum;
    }
    bell_max = (size_t)N;
}

/* The requested OEIS sequence A340822. */
seq_t a(idx_t n)
{
    /* Bell numbers up to 2*n are needed because of the term
       Bell(2*n-k). */
    ensure_bell_up_to(2 * n);

    seq_t result   = 0;
    seq_t binom    = 1;          /* binomial(n,0)          */
    seq_t power_n  = 1;          /* n^0                   */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom * power_n * bell_cache[2 * n - k];
        result += term;

        if (k < n) {
            binom   = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
            power_n = power_n * (seq_t)n;
        }
    }
    return result;
}

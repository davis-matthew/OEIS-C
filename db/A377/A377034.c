/*
 * A377034  Antidiagonal-sums of the array A377033(n,k) = n-th term of the
 *          k-th differences of the composite numbers (A002808).
 * https://oeis.org/A377034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0}^{n-1} Δ^k composite(n−k)
 * Range:     i8 n <= 15, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies required from the build environment */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple primality test for small integers: used to generate composites */
static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

/* Cache of composite numbers (1‑based indexing: comp[0] == 4).  */
static seq_t *comp = NULL;
static size_t comp_len = 0;            /* number of composites currently cached */
static uint64_t next_candidate = 4ULL; /* candidate to test for composite */

/* Ensure that at least n composite numbers are cached. */
static void ensure_composites(size_t n)
{
    while (comp_len < n) {
        /* Find the next composite number. */
        while (is_prime_small(next_candidate)) ++next_candidate;

        /* Grow the array by one element. */
        size_t new_len = comp_len + 1;
        seq_t *tmp = (seq_t *)realloc(comp, new_len * sizeof(seq_t));
        comp = tmp;
        comp[comp_len] = (seq_t)next_candidate;
        ++comp_len;
        ++next_candidate;
    }
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset 1; return 0 for n==0 */

    size_t N = (size_t)n;
    ensure_composites(N);

    /* Work array for successive differences.  */
    seq_t *dif = (seq_t *)malloc(N * sizeof(seq_t));
    if (!dif) return (seq_t)0;  /* simple error handling */

    for (size_t i = 0; i < N; ++i)
        dif[i] = comp[i];

    seq_t sum = 0;

    for (size_t k = 0; k < N; ++k) {
        /* Add the element on the current antidiagonal. */
        sum += dif[N - 1 - k];

        if (k == N - 1) break;          /* no more differences to form */

        /* Form the next level of forward differences in-place. */
        size_t len = N - k;              /* current effective length */
        for (size_t i = 0; i < len - 1; ++i)
            dif[i] = dif[i + 1] - dif[i];
    }

    free(dif);
    return sum;
}

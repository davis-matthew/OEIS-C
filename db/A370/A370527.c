/*
 * A370527  Triangle read by rows: T(n,k) = number of permutations of [n]
 *          having exactly one adjacent k-cycle. (n>=1, 1<=k<=n).
 * https://oeis.org/A370527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A370527.
   a(n) is the n-th term of the triangle read by rows,
   where T(N,k) = Σ_{j=0}^{⌊N/k⌋-1} (-1)^j * (N-(k-1)(j+1))! / j!.
   The function obtains the row index N and column k from n, precomputes
   factorials up to N and evaluates the above sum. */

#include <stdlib.h>
#include <stdbool.h>

/* Global cache of factorials: fac[i] = i! */
static seq_t *fac = NULL;   /* factorial cache */
static idx_t fac_max = 0;   /* highest index computed */

/* Allocate factorials up to `up_to` and update the cache. */
static void ensure_factorial(idx_t up_to)
{
    if (fac == NULL) {
        fac = (seq_t *)malloc(sizeof(seq_t));
        if (!fac) abort();
        fac[0] = (seq_t)1;
        fac_max = 0;
    }
    if (fac_max >= up_to) return;

    size_t new_cap = (size_t)(up_to + 1);
    seq_t *tmp = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    fac = tmp;

    for (idx_t i = fac_max + 1; i <= up_to; ++i) {
        fac[i] = fac[i-1] * (seq_t)i;
    }
    fac_max = up_to;
}

/* Main sequence function requested by the build system. */
seq_t a(idx_t idx)
{
    /* Map linear index `idx` to triangle coordinates (N,k). */
    idx_t N = 1;
    idx_t tri = 1;  /* triangular number N*(N+1)/2 */
    while (tri < idx) {
        ++N;
        tri = (idx_t)(N * (N + 1) / 2);
    }
    idx_t prev_tri = ((N - 1) * N) / 2;
    idx_t k = idx - prev_tri;  /* 1 <= k <= N */

    /* Compute factorials up to N. */
    ensure_factorial(N);

    /* Evaluate the alternating sum for T(N,k). */
    idx_t max_j = N / k;  /* floor(N/k) */
    seq_t result = (seq_t)0;

    for (idx_t j = 0; j < max_j; ++j) {
        idx_t argument = N - (k - 1) * (j + 1);
        seq_t term = fac[argument];
        term /= fac[j];          /* divide by j! */
        if ((j & 1) == 0)
            result += term;
        else
            result -= term;
    }
    return result;
}

/*
 * A333295  Triangle read by rows: T(m,n) = Sum_{1 <= i <= m, 1 <= j <= n,
 *          gcd{i,j}=1} 1, where m >= n >= 1.
 * https://oeis.org/A333295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{1 <= i <= m, 1 <= j <= n, gcd(i,j)=1} 1 where (m,n)
 *            are the row and column indices of the n‑th term in the triangle
 *            read by rows. The value is computed by Möbius inversion: T(m,n)
 *            = sum_{d=1}^{min(m,n)} mu(d) * floor(m/d) * floor(n/d). The
 *            Möbius function mu(d) is maintained up to the...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int *mu_arr = NULL;      /* Möbius values, indices 0..max */
static size_t mu_alloc = 0;     /* number of elements allocated */

/* Recompute the Möbius function up to 'limit' (inclusive).  It replaces
   any previously computed array. */
static void ensure_mu(size_t limit)
{
    if (limit + 1 <= mu_alloc)
        return;

    int *mu_new = (int *)malloc((limit + 1) * sizeof(int));
    if (!mu_new) abort();

    mu_new[0] = 0;
    mu_new[1] = 1;

    /* Linear sieve for mu */
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    if (!is_comp) abort();
    int *primes = (int *)malloc((limit + 1) * sizeof(int));
    if (!primes) abort();
    size_t primes_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (int)i;
            mu_new[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt && (size_t)primes[j] * i <= limit; ++j) {
            size_t ip = i * (size_t)primes[j];
            is_comp[ip] = 1;
            if (i % primes[j] == 0) {
                mu_new[ip] = 0;
                break;
            } else {
                mu_new[ip] = -mu_new[i];
            }
        }
    }

    free(is_comp);
    free(primes);
    free(mu_arr);
    mu_arr = mu_new;
    mu_alloc = limit + 1;
}

/* Compute the triangle entry T(m,n) with m >= n.  The arithmetic
   is performed in signed long long to accommodate any temporary
   negative contributions from the Möbius function. */
static seq_t triangle_entry(idx_t m, idx_t n)
{
    idx_t dmax = n; /* n <= m by construction */
    long long sum = 0;
    ensure_mu((size_t)dmax);
    for (idx_t d = 1; d <= dmax; ++d) {
        int mu = mu_arr[d];
        if (mu == 0) continue;
        long long term = (long long)mu *
                         (long long)(m / d) *
                         (long long)(n / d);
        sum += term;
    }
    return (seq_t)sum; /* sum is always non‑negative */
}

/* Main sequence function: returns the n‑th term of the flattened
   triangle. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n==0 */

    /* Find row m such that m(m+1)/2 >= n */
    idx_t m = 1;
    seq_t tri = 1; /* m(m+1)/2 */
    while (tri < (seq_t)n) {
        ++m;
        tri = (seq_t)m * (m + 1) / 2;
    }

    /* Column index within that row */
    seq_t prev_tri = (seq_t)(m - 1) * m / 2;
    idx_t col = (idx_t)((seq_t)n - prev_tri);

    /* Ensure col <= m; this is always true for valid inputs */
    if (col > m) col = m;

    return triangle_entry(m, col);
}

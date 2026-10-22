/*
 * A348075  Triangular array read by rows: T(n,k) is the number of
 *          derangements whose shortest cycle has exactly k nodes; n >= 1, 1
 *          <= k <= n.
 * https://oeis.org/A348075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th term of the triangle read by rows) where
 *            T(n,k) is the number of derangements of n with shortest cycle
 *            exactly k. For k≥2: T(n,k) = f(n,k) – f(n,k+1), where f(n,k)
 *            counts permutations with all cycle lengths ≥ k. Recurrence for
 *            f(n,k): f(0,k) = 1 f(n,k) = Σ_{i=k}^{n} C(n‑1,i‑1) ·...
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t binom(idx_t N, idx_t K, seq_t *fact)
{
    if (K > N) return (seq_t)0;
    idx_t a = N - K;
    seq_t denom = fact[K] * fact[a];
    return fact[N] / denom;
}

static seq_t compute_f(idx_t n, idx_t k, seq_t *fact)
{
    if (k > n)
        return (seq_t)0;
    /* dynamic array for f[0..n] */
    seq_t *f = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    f[0] = 1;
    for (idx_t curr = 1; curr <= n; ++curr) {
        if (curr < k) {
            f[curr] = 0;
            continue;
        }
        seq_t sum = 0;
        /* sum over i = k .. curr */
        for (idx_t i = k; i <= curr; ++i) {
            seq_t bin = binom(curr - 1, i - 1, fact);
            seq_t term = bin * fact[i - 1];
            term *= f[curr - i];
            sum += term;
        }
        f[curr] = sum;
    }
    seq_t result = f[n];
    free(f);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 is the first entry; n=0 is unused */
        return (seq_t)0;

    /* locate row r and column k for index n */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    idx_t prev_sum = sum - r;
    idx_t k_col = n - prev_sum;   /* 1 ≤ k_col ≤ r */

    /* k == 1 => shortest cycle cannot be 1 in a derangement */
    if (k_col == 1)
        return (seq_t)0;

    /* factorials up to r */
    seq_t *fact = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    fact[0] = 1;
    for (idx_t i = 1; i <= r; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* compute f(r, k_col) */
    seq_t fk = compute_f(r, k_col, fact);

    /* compute f(r, k_col+1) */
    seq_t fk1;
    if (k_col + 1 > r)
        fk1 = 0;                /* k+1 > n ⇒ no permutations */
    else
        fk1 = compute_f(r, k_col + 1, fact);

    free(fact);
    return fk - fk1;
}

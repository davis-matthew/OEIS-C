/*
 * A308083  G.f. A(x) satisfies: A(x) = x + x^2 + x^3 + x^4 + x^5 * (1 +
 *          Sum_{i>=1} Sum_{j>=1} A(x^(i*j))).
 * https://oeis.org/A308083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=a(4)=a(5)=1; a(n+5) = Σ_{d|n} τ(n/d) · a(d) for n
 *            ≥ 1 (τ = number of divisors)
 * Range:     u8 n <= 52, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;         /* cache of a(i), index i ≥ 0 (0 unused) */
static seq_t *tau  = NULL;          /* τ(k) for k ≥ 0 (0 unused) */
static size_t cache_cap = 0;        /* allocated size of cache, including index 0 */
static size_t tau_cap   = 0;        /* allocated size of tau, including index 0 */
static idx_t  tau_limit = 0;        /* currently computed τ for values ≤ tau_limit */

/* enlarge an array to be able to index up to `need-1` (index 0 is unused) */
static void ensure_capacity(idx_t need, seq_t **arr, size_t *cap) {
    if ((size_t)need <= *cap) return;
    size_t new_cap = (*cap > 0) ? *cap : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    for (idx_t i = *cap; i < (idx_t)new_cap; ++i) (*arr)[i] = 0;
    *cap = new_cap;
}

/* compute τ(k) for 1 ≤ k ≤ m (if not already up‑to‑date). */
static void recompute_tau(idx_t m) {
    if (m <= tau_limit) return;
    ensure_capacity(m + 1, &tau, &tau_cap);
    for (idx_t i = 1; i <= m; ++i) tau[i] = 0;
    for (idx_t d = 1; d <= m; ++d)
        for (idx_t k = d; k <= m; k += d)
            ++tau[k];
    tau_limit = m;
}

/* compute all a(i) for i ≤ n (bottom‑up). */
static void compute_upto(idx_t n) {
    if (n == 0) return;
    ensure_capacity(n + 1, &cache, &cache_cap);

    /* base cases */
    for (idx_t i = 1; i <= 5 && i <= n; ++i) cache[i] = 1;

    if (n <= 5) return;                          /* nothing else to compute */

    recompute_tau(n - 5);                         /* we need τ for values up to n‑5 */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t sum = 0;
        idx_t m = i - 5;                         /* m ≥ 1 */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t d1 = d;
                idx_t d2 = m / d;
                sum += tau[d2] * cache[d1];
                if (d1 != d2)
                    sum += tau[d1] * cache[d2];
            }
        }
        cache[i] = sum;
    }
}

/* public entry point */
seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n<1, but safe to return 0 */
    compute_upto(n);
    return cache[n];
}

/*
 * A280075  Partial products of A211776 (Product_{d|n} tau(d)).
 * https://oeis.org/A280075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1..n} Π_{d|i} τ(d) (τ = number of divisors)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The build will provide seq_t and idx_t. */
static idx_t  max_n_computed = 0;   /* largest n already processed         */
static idx_t *spf            = NULL;/* smallest prime factor array         */
static size_t spf_size        = 0;
static uint32_t *tau         = NULL;/* divisor‑count array                 */
static size_t tau_size        = 0;
static seq_t *cache          = NULL;/* cumulative product cache           */
static size_t cache_size     = 0;

/* Make sure all static arrays are large enough for 0..n */
static void ensure_size(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (spf_size < need) {
        idx_t *tmp = (idx_t *)realloc(spf, need * sizeof(idx_t));
        spf = tmp;
        spf_size = need;
    }
    if (tau_size < need) {
        uint32_t *tmp = (uint32_t *)realloc(tau, need * sizeof(uint32_t));
        tau = tmp;
        tau_size = need;
    }
    if (cache_size < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        cache_size = need;
    }
}

/* Build the smallest‑prime‑factor array up to n. */
static void build_spf(idx_t n)
{
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {                      /* prime */
            spf[i] = i;
            if ((unsigned long long)i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* Compute τ[1..n] using the spf array.  τ[1] = 1. */
static void compute_tau(idx_t n)
{
    tau[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t e = 1;
        idx_t t = i / p;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        tau[i] = tau[t] * (e + 1);
    }
}

/* Multiply all τ(d) for d|k.  The product fits in seq_t. */
static seq_t prod_tau_divisors(idx_t k)
{
    seq_t prod = (seq_t)1;
    idx_t lim = (idx_t)((unsigned long long)k * 1.0);   /* avoid 64‑bit overflow */
    for (idx_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            prod *= (seq_t)tau[d];
            idx_t d2 = k / d;
            if (d2 != d)
                prod *= (seq_t)tau[d2];
        }
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below the offset */

    /* Make sure the static arrays can hold up to n. */
    ensure_size(n);

    /* Initialise the cache base if this is the first call. */
    if (max_n_computed == 0) {
        cache[0] = (seq_t)1;      /* Π_{i=1..0} = 1 */
    }

    /* If we already have the result, just return it. */
    if (n <= max_n_computed)
        return cache[n];

    /* Extend the SPF and τ arrays. */
    build_spf(n);
    compute_tau(n);

    /* Compute a(k) for each new k > max_n_computed. */
    for (idx_t k = max_n_computed + 1; k <= n; ++k) {
        seq_t prod = prod_tau_divisors(k);
        cache[k] = cache[k - 1] * prod;
    }

    max_n_computed = n;
    return cache[n];
}

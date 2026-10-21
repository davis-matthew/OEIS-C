/*
 * A292769  Partial sums of A051612.
 * https://oeis.org/A292769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} (1 - μ(k)) * ⌊n/k⌋·(⌊n/k⌋+1)/2 where μ is the
 *            Möbius function
 * Range:     u8 n <= 21, u16 n <= 355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache for the Möbius function.  mu_cache[0] is unused. */
static int8_t *mu_cache = NULL;
static idx_t mu_limit = 0;

/* Build μ(1)…μ(n) with a linear‑sieve (O(n)).  The array is
   reallocated if a larger n is requested. */
static void ensure_mu_up_to(idx_t n)
{
    if (n <= mu_limit)
        return;

    size_t size = (size_t)(n + 1);
    int8_t *new_mu = (int8_t *)malloc(size * sizeof(int8_t));
    if (!new_mu)
        abort();

    bool *is_comp = (bool *)calloc(size, sizeof(bool));
    if (!is_comp) {
        free(new_mu);
        abort();
    }

    /* Even for n=10000 the number of primes is < 2000, so a simple
       array big enough for n elements is fine. */
    int *primes = (int *)malloc(size * sizeof(int));
    if (!primes) {
        free(new_mu);
        free(is_comp);
        abort();
    }

    int prime_cnt = 0;
    new_mu[1] = 1;                    /* μ(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[(size_t)i]) {
            primes[prime_cnt++] = (int)i;
            new_mu[(size_t)i] = -1;  /* μ(p) = -1 for primes */
        }
        for (int pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = (idx_t)primes[pi];
            /* Avoid overflow: if i > n / p we would exceed n. */
            if (i > n / p)
                break;
            idx_t ip = i * p;
            is_comp[(size_t)ip] = true;
            if (i % p == 0) {
                new_mu[(size_t)ip] = 0;   /* square factor → μ = 0 */
                break;
            } else {
                new_mu[(size_t)ip] = -(new_mu[(size_t)i]);
            }
        }
    }

    free(is_comp);
    free(primes);
    free(mu_cache);
    mu_cache = new_mu;
    mu_limit = n;
}

/* Return the n‑th term of OEIS sequence A292769. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1 */

    ensure_mu_up_to(n);

    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int8_t mu_val = mu_cache[(size_t)d];
        if (mu_val == 1)          /* (1 - μ) == 0 → skip */
            continue;

        seq_t mult = 1 - (seq_t)mu_val;      /* 1 or 2 depending on μ(d) */
        seq_t q   = (seq_t)(n / d);
        seq_t add = mult * q * (q + 1) / 2;
        result += add;
    }
    return result;
}

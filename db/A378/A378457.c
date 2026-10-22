/*
 * A378457  Difference between n and the greatest prime power <= n, allowing
 *          1.
 * https://oeis.org/A378457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A031218(n) (greatest prime power <= n, 1 allowed)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of greatest prime powers up to the largest n cached. */
static seq_t *gp_cache = NULL;   /* gp_cache[k] = greatest prime power <= k */
static idx_t  cache_cap = 0;     /* highest index cached */

/* Recompute gp_cache for all k up to n.  Happens only when n > cache_cap. */
static void recompute_gp_cache(idx_t n)
{
    /* Allocate new array.  Explicit cast required for C++. */
    seq_t *new_gp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_gp) exit(1);          /* unrecoverable memory error */

    /* Initialise: 1 is a prime power, others set to 1 (will be overridden). */
    new_gp[0] = (seq_t)0;
    new_gp[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        new_gp[i] = (seq_t)1;

    /* Sieve of Eratosthenes for primes up to n.  Explicit cast for C++. */
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!is_prime) { free(new_gp); exit(1); }

    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (idx_t p = 2; p <= n / 2; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t m = p * p; m <= n; m += p)
            is_prime[m] = false;
    }

    /* Mark each exact prime power with its value. */
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p]) continue;
        idx_t val = p;
        while (true) {
            new_gp[val] = (seq_t)val;         /* value itself */
            if (val > n / p) break;          /* avoid overflow */
            val *= p;
        }
    }

    /* Propagate the running maximum to ensure new_gp[i] is the greatest
       prime power <= i for all i. */
    for (idx_t i = 2; i <= n; ++i)
        if (new_gp[i] < new_gp[i - 1])
            new_gp[i] = new_gp[i - 1];

    /* Replace old cache. */
    free(is_prime);
    free(gp_cache);
    gp_cache = new_gp;
    cache_cap = n;
}

/* Ensure gp_cache covers at least n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    recompute_gp_cache(n);
}

/* Public sequence function.  a(1) = 0, a(n) = n - greatest prime power <= n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety; not defined for 0 */
    ensure_cache(n);
    return (seq_t)n - gp_cache[n];
}

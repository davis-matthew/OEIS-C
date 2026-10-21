/*
 * A248768  Greatest k such that k^5 divides n!
 * https://oeis.org/A248768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} p^{⌊v_p(n!)/5⌋} , where v_p(n!) = ∑_{i≥1} ⌊n/p^i⌋
 * Range:     u8 n <= 24, u16 n <= 37, u32 n <= 62, u64 n <= 97, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes the list of primes up to n on demand
   and then evaluates the exponent for each prime using the standard
   Legendre formula.  Multiplication is performed directly on seq_t,
   thus working for all supported integer classes. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for primes */

static idx_t sieve_limit = 0;          /* highest n for which primes are known */
static uint64_t *prime_list = NULL;    /* array of primes up to sieve_limit   */
static size_t prime_count = 0;         /* number of primes stored             */

/* Ensure prime_list contains all primes ≤ n. */
static void
ensure_primes(idx_t n)
{
    if (n <= sieve_limit) return;          /* already enough */

    /* Simple sieve of Eratosthenes for the new range */
    bool *is_prime = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_prime) return;                 /* allocation failed → nothing to do */

    for (idx_t i = 2; i <= n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p) is_prime[m] = false;
        }
    }

    /* Count primes and build the list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[i]) ++cnt;

    uint64_t *tmp = (uint64_t*)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!tmp) { free(is_prime); return; }  /* allocation failed */
    prime_list = tmp;
    prime_count = cnt;

    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[i]) prime_list[idx++] = i;

    free(is_prime);
    sieve_limit = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;  /* undefined for n<1, return 1 as guard */

    ensure_primes(n);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > n) break;

        /* Compute v_p(n!) via Legendre's formula */
        idx_t power = 0;
        idx_t div = p;
        while (div <= n) {
            power += n / div;
            /* avoid overflow: div <= n, n≤UINT64_MAX so next div safe */
            div *= p;
        }

        idx_t exp5 = power / 5;           /* floor(v_p/5) */
        for (idx_t k = 0; k < exp5; ++k) result *= (seq_t)p;
    }
    return result;
}

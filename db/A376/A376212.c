/*
 * A376212  a(n) is the least k such that A001615(k) = 2*n, or -1 if there is
 *          no such k, where A001615 is the Dedekind psi function.
 * https://oeis.org/A376212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) is the least k such that the Dedekind
 *            ψ(k)=k*∏_{p|k}(1+1/p)=2*n, * or -1 if no such k exists. This
 *            implementation searches k for 1≤k<2*n * and factors each k using
 *            a simple trial‑division sieve
 * Range:     i8 n <= 68, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------
   Prime sieve helpers
   We maintain a static list of primes; if a larger limit is needed
   we rebuild the list. The sieve is very small because the
   maximum k we encounter is < 2*n (≤ 20000 for n≤10000). */
static uint32_t *primes = NULL;      /* array of primes */
static size_t    prime_count = 0;    /* number of primes */
static uint32_t  prime_limit  = 0;   /* maximum value included in primes */

static void ensure_prime_limit(uint32_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate a small boolean array for the sieve */
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_composite) return;   /* allocation failed – should not happen */

    /* Simple Sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i)
        if (!is_composite[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;

    /* Count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    /* Allocate the prime array */
    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) {
        free(is_composite);
        return;   /* allocation failed – should not happen */
    }

    /* Store primes */
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) new_primes[idx++] = i;

    /* Replace old prime list */
    free(primes);
    primes = new_primes;
    prime_count = cnt;
    prime_limit  = limit;

    free(is_composite);
}

/* ------------------------------------------------------------
   Dedekind ψ function in the algorithmic form:
   ψ(k) = k * ∏_{p|k,(p distinct)} (p+1)/p
   All calculations are performed with unsigned 64‑bit integers,
   which are sufficient for k up to 2*10 000. */
static uint64_t dedekind_psi(uint64_t k)
{
    if (k == 1) return 1;  /* ψ(1)=1 */
    uint64_t psi = k;
    uint64_t m   = k;

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            psi = psi * ((uint64_t)p + 1ULL) / (uint64_t)p;
            while (m % p == 0) m /= p;
        }
    }

    if (m > 1) {  /* remaining prime factor > sqrt(k) */
        psi = psi * (m + 1ULL) / m;
    }

    return psi;
}

/* ------------------------------------------------------------
   Main function required by the specification. */
seq_t a(idx_t n)
{
    /* The target value is 2*n, computed with enough width. */
    uint64_t target = 2ULL * (uint64_t)n;

    /* Upper bound for k: we examine 1 ≤ k < 2*n. */
    uint64_t limit = 2ULL * (uint64_t)n;
    if (limit <= 1) return (seq_t)-1;   /* n = 0 never occurs (offset 1) */

    /* Ensure we have primes at least up to the maximum possible k. */
    ensure_prime_limit((uint32_t)limit);

    for (uint64_t k = 1; k < limit; ++k) {
        uint64_t psi = dedekind_psi(k);
        if (psi == target) return (seq_t)k;
    }
    return (seq_t)-1;
}

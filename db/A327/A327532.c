/*
 * A327532  Indicator function for numbers whose prime indices are relatively
 *          prime (A289509).
 * https://oeis.org/A327532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8980, u16 n <= 8980, u32 n <= 8980, u64 n <= 8980,
 *            u128 n <= 8980, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indicator function for numbers whose prime indices are relatively prime
   (OEIS A327532).  a(n)=1 iff gcd{ pi(p) : p divides n } = 1,
   where pi(p) is the 1‑based index of prime p (pi(2)=1, pi(3)=2, …).
   a(1)=0.  The function works for any seq_t and idx_t defined by the build
   and is independent of the order of calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* -------------------------------------------------------------------- */
/* Prime sieve – generates all primes up to a limit and stores them in
   prime_list.  prime_list is sorted ascending and prime_cnt is the
   number of primes.  prime_lim is the largest limit already sieved. */

static uint64_t *prime_list = NULL;
static size_t      prime_cnt = 0;
static idx_t       prime_lim = 0;

static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_lim) return;

    /* Simple sieve of Eratosthenes */
    bool *is_prime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return;            /* allocation failure – abort silently */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes to allocate prime_list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_list) { free(is_prime); return; }

    cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_list[cnt++] = (uint64_t)i;

    free(prime_list);
    free(is_prime);
    prime_list = new_list;
    prime_cnt  = cnt;
    prime_lim  = limit;
}

/* -------------------------------------------------------------------- */
/* Binary search: returns the 1‑based index of prime p (assumed to be present). */

static seq_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else if (prime_list[mid] > p) hi = mid;
        else return (seq_t)(mid + 1);  /* indices are 1‑based */
    }
    return (seq_t)0;  /* should not happen for a valid prime p */
}

/* -------------------------------------------------------------------- */
/* Factorisation – collects distinct primes dividing n.  primes_out
   must have space for at least 64 elements. */

static void
factor(uint64_t n, uint64_t *primes_out, size_t *pr_count)
{
    *pr_count = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= rem; ++i) {
        uint64_t p = prime_list[i];
        if (rem % p == 0) {
            primes_out[(*pr_count)++] = p;
            while (rem % p == 0) rem /= p;
        }
    }
    if (rem > 1) primes_out[(*pr_count)++] = rem;
}

/* -------------------------------------------------------------------- */
/* Main sequence function. */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* Ensure the prime table covers n – needed for factorisation and indices. */
    ensure_primes(n);

    uint64_t primes[64];
    size_t   fp_cnt;
    factor((uint64_t)n, primes, &fp_cnt);

    seq_t g = 0;   /* gcd(0, x) = x */
    for (size_t i = 0; i < fp_cnt; ++i) {
        seq_t idx = prime_index(primes[i]);
        g = gcd(g, idx);
        if (g == (seq_t)1) break;   /* early exit */
    }

    return (g == (seq_t)1) ? (seq_t)1 : (seq_t)0;
}

/*
 * A283302  Maximum number of pairs of primes (p,q) such that p < q <=
 *          prime(n) and p + q = constant.
 * https://oeis.org/A283302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 877, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A283302
    a(n) = maximum multiplicity of a sum p+q, where
    p,q are distinct primes among the first n primes, p<q.
    Implementation: brute‑force enumeration of all pairs of the
    first n primes, counting frequencies of their sums, and returning
    the largest count.  Complexity O(n²) with a small constant;
    sufficient for n up to about 10 000. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/*  Prime sieve (static cache)                                 */
/* ------------------------------------------------------------ */
static const size_t PRIME_LIMIT = 200000;          /* > prime(10000) */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_prime_list(void)
{
    if (prime_list != NULL)
        return;               /* already initialized */

    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime)
        return;               /* allocation failure – not expected */

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        return;           /* allocation failure – not expected */
    }

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint32_t)i;

    prime_count = cnt;
    free(is_prime);
}

/* ------------------------------------------------------------ */
/*  Sequence function                                           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;              /* a(1) = 0 */

    init_prime_list();

    /* guard against an out‑of‑range request */
    if ((size_t)n > prime_count)
        return 0;              /* should not happen in the test set */

    const size_t N = (size_t)n;
    const uint32_t *pr = prime_list;

    /* maximum possible sum of two distinct primes in the set */
    uint32_t max_sum = pr[N - 1] + pr[N - 2];

    /* frequency table for sums */
    size_t freq_size = (size_t)max_sum + 1;
    uint32_t *freq = (uint32_t*)calloc(freq_size, sizeof(uint32_t));
    if (!freq)
        return 0;              /* allocation failure – not expected */

    uint32_t max_count = 0;

    for (size_t i = 0; i < N; ++i) {
        uint32_t pi = pr[i];
        for (size_t j = i + 1; j < N; ++j) {
            uint32_t s = pi + pr[j];
            uint32_t c = ++freq[s];
            if (c > max_count)
                max_count = c;
        }
    }

    free(freq);
    return (seq_t)max_count;
}

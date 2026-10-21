/*
 * A251595  Distinct terms in A251416.
 * https://oeis.org/A251595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n-4) for n > 11 a(n) = 2,3,4,5,6,7,10,11,13,17,18
 *            for 1 ≤ n ≤ 11 (prime(k) is the k‑th prime, prime(1)=2)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ---- prime generation utilities ------------------------------------ */

static idx_t estimate_limit(idx_t k)
{
    /* Rosser’s bound: p_k < k (log k + log log k) for k >= 6 */
    if (k < 6) return 15;
    double dk = (double)k;
    double est = dk * (log(dk) + log(log(dk))) + 3.0;
    return (idx_t)(est) + 1;
}

/* Produce all primes ≤ limit, return array and count. */
static void generate_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    /* Allocate sieve */
    char *sieve = (char *)malloc((size_t)(limit + 1));
    if (!sieve) exit(EXIT_FAILURE);
    /* 0 and 1 are not prime */
    sieve[0] = sieve[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) sieve[i] = 1;

    idx_t sqrt_lim = (idx_t)floor(sqrt((double)limit));
    for (idx_t p = 2; p <= sqrt_lim; ++p) {
        if (sieve[p]) {
            for (idx_t m = p * p; m <= limit; m += p) sieve[m] = 0;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) ++count;

    /* Allocate output array */
    uint64_t *primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);

    /* Fill array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) primes[idx++] = (uint64_t)i;

    free(sieve);
    *primes_out = primes;
    *cnt_out = count;
}

/* -------------------------------------------------------------------- */

static uint64_t *primes = NULL;
static size_t primes_count = 0;
static idx_t primes_limit = 0;

/* Ensure at least 'need' primes are available */
static void ensure_primes_up_to(idx_t need)
{
    if (need <= primes_count) return;

    idx_t limit = estimate_limit(need);

    /* Repeatedly enlarge limit until we get enough primes */
    while (true) {
        uint64_t *new_primes = NULL;
        size_t new_count = 0;
        generate_primes_upto(limit, &new_primes, &new_count);

        if (!new_primes) exit(EXIT_FAILURE);

        if (new_count >= (size_t)need) {
            /* Replace old list */
            free(primes);
            primes = new_primes;
            primes_count = new_count;
            primes_limit = limit;
            break;
        }

        /* Not enough, increase limit and try again */
        free(new_primes);
        limit = limit * 2 + 100;
    }
}

/* Sequence function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1 */

    static const seq_t first11[] = {
        (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)5, (seq_t)6,
        (seq_t)7, (seq_t)10, (seq_t)11, (seq_t)13, (seq_t)17, (seq_t)18
    };

    if (n <= 11) return first11[n - 1];

    /* n > 11: need the (n-4)-th prime */
    idx_t need = n - 4;
    ensure_primes_up_to(need);
    return (seq_t)primes[need - 1];
}

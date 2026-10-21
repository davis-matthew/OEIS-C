/*
 * A286515  a(n) = denominator(Bernoulli_{n}(x)) / denominator(Bernoulli_{n}).
 * https://oeis.org/A286515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator( Bₙ(x) ) / denominator( Bₙ ) * * The
 *            denominator of a Bernoulli polynomial Bₙ(x) equals the product *
 *            of all primes p such that (p-1) divides the index m (=n-k) of
 *            the * Bernoulli number B_m that appears in the k‑th term of the
 *            polynomial, * provided that p does not divide the...
 * Range:     u8 n <= 20, u16 n <= 60, u32 n <= 180, u64 n <= 420,
 *            u128 n <= 1800, big any n
 * Helpers:   binom_divisible_by_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom_divisible_by_p.h"

/* simple prime sieve up to `limit`; result stored in *primes_out */
/* the array is reallocated if `limit` grows; it is freed on next rebuild */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool *)malloc(sz);
    if (!sieve) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (size_t i = 0; i < sz; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
        if (sieve[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[(size_t)j] = false;

    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (sieve[(size_t)i])
            ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(sieve);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t pos = 0;
    for (size_t i = 2; i < sz; ++i)
        if (sieve[(size_t)i])
            primes[pos++] = (uint64_t)i;

    free(sieve);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* prime cache, rebuilt only when needed */
    static uint64_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (prime_limit < n + 1) {              /* rebuild primes table */
        if (prime_arr)
            free(prime_arr);
        build_primes_upto(n + 1, &prime_arr, &prime_cnt);
        prime_limit = n + 1;
    }

    seq_t result = 1;

    /* loop over all primes ≤ n+1 */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = prime_arr[pi];
        /* skip primes that already divide the denominator of Bₙ */
        if (p == 2) {
            if (n == 1 || (n % 2 == 0 && n >= 2))
                continue;
        } else {
            if (n % 2 == 0 && n >= 2 && n % (p - 1) == 0)
                continue;
        }

        /* step = p-1 : m must be a positive multiple of step */
        idx_t step = (idx_t)(p - 1);
        bool found = false;

        /* m ranges from step, 2*step, … ≤ n  */
        for (idx_t m = step; !found && m <= n; m += step) {
            /* m ≥ 1 ensures B_m denominator contains p */
            if (!binom_divisible_by_p(n, m, p)) {
                found = true;               /* p contributes */
            }
        }

        if (found)
            result = result * (seq_t)p;
    }

    return result;
}

/*
 * A247474  Numbers formed by concatenating two successive primes and their
 *          sum.
 * https://oeis.org/A247474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal integer obtained by concatenating * the n-th
 *            prime, the (n+1)-th prime, and the sum of these two primes. *
 *            Example: n = 1 → primes 2,3, sum 5 → a(1) = 235
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 94, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* dynamic cache of primes – grows as needed */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

/* Ensure that at least 'need' primes are available in prime_cache. */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;

    /* Allocate or grow the storage. */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cap < (size_t)need) {
        while (prime_cap < (size_t)need)
            prime_cap <<= 1;
        prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
    }

    /* Generate missing primes. */
    for (size_t i = prime_cnt; i < (size_t)need; ++i) {
        uint64_t cand = (i == 0) ? 2ULL : prime_cache[i - 1] + 1ULL;
        bool is_prime = false;
        do {
            is_prime = true;
            for (size_t j = 0; j < i; ++j) {
                uint64_t p = prime_cache[j];
                if (p * p > cand)
                    break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (!is_prime)
                ++cand;
        } while (!is_prime);
        prime_cache[i] = cand;
    }
    prime_cnt = (size_t)need;
}

/* Compute 10^e as seq_t (e >= 0). */
static seq_t pow10_seq(int e)
{
    seq_t r = (seq_t)1;
    seq_t ten = (seq_t)10;
    while (e-- > 0)
        r *= ten;
    return r;
}

/* Number of decimal digits in x (x > 0). */
static int digit_len_seq(seq_t x)
{
    int len = 0;
    while (x != (seq_t)0) {
        x /= (seq_t)10;
        ++len;
    }
    return len;
}

/* Sequence function requested by the problem. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset < 1 is undefined – behaviour irrelevant */
        return (seq_t)0;

    /* We need primes at positions n and n+1 (1‑based). */
    ensure_primes(n + 1);

    seq_t p1  = (seq_t)prime_cache[n - 1];
    seq_t p2  = (seq_t)prime_cache[n];
    seq_t sum = p1 + p2;

    int len_p2  = digit_len_seq(p2);
    int len_sum = digit_len_seq(sum);

    seq_t pow_total = pow10_seq(len_p2 + len_sum);
    seq_t pow_sum   = pow10_seq(len_sum);

    seq_t result = p1 * pow_total + p2 * pow_sum + sum;
    return result;
}

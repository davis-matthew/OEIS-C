/*
 * A386596  a(n) = Sum_{d|n} d * pi(n/d).
 * https://oeis.org/A386596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} d * pi(n/d)
 * Range:     u8 n <= 59, u16 n <= 4619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t is_prime_size = 0;
static bool *is_prime = NULL;

static idx_t pi_cache_size = 0;
static seq_t *pi_cache = NULL;

/* Build or extend the sieve up to n. */
static void _ensure_sieve(idx_t n)
{
    if (n <= is_prime_size)
        return;
    bool *new_arr = (bool*)realloc(is_prime, (n + 1) * sizeof(bool));
    if (!new_arr && n > 0)
        abort();
    is_prime = new_arr;
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }
    is_prime_size = n;
}

/* Build or extend the prime‑count cache up to n. */
static void _ensure_pi_cache(idx_t n)
{
    _ensure_sieve(n);
    if (n <= pi_cache_size)
        return;
    seq_t *new_arr = (seq_t*)realloc(pi_cache, (n + 1) * sizeof(seq_t));
    if (!new_arr && n > 0)
        abort();
    pi_cache = new_arr;
    if (pi_cache_size == 0)
        pi_cache[0] = 0;
    for (idx_t i = pi_cache_size + 1; i <= n; ++i)
        pi_cache[i] = pi_cache[i - 1] + (is_prime[i] ? (seq_t)1 : (seq_t)0);
    pi_cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n=0, return 0 for safety */
    _ensure_pi_cache(n);
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            seq_t pi1 = pi_cache[d2];
            sum += (seq_t)d1 * pi1;
            if (d2 != d1) {
                seq_t pi2 = pi_cache[d1];
                sum += (seq_t)d2 * pi2;
            }
        }
    }
    return sum;
}

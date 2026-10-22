/*
 * A304066  a(n) = Sum_{k=1..n} k*floor(n/prime(k)).
 * https://oeis.org/A304066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*floor(n/prime(k))
 * Range:     u8 n <= 46, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

static void ensure_primes_upto(idx_t limit) {
    if (limit <= primes_limit) return;
    idx_t new_limit = limit;
    uint8_t *is_prime = (uint8_t *)malloc((new_limit + 1) * sizeof(uint8_t));
    if (!is_prime) return; /* allocation failure – ignore (caller will get 0) */
    for (idx_t i = 0; i <= new_limit; ++i) is_prime[i] = 1;
    if (new_limit >= 0) { /* guard against negative limits, though unlikely */
        is_prime[0] = 0;
        if (new_limit >= 1) is_prime[1] = 0;
    }
    for (idx_t p = 2; p * p <= new_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= new_limit; m += p) is_prime[m] = 0;
        }
    }
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i) if (is_prime[i]) ++cnt;
    idx_t *new_primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_primes) {
        free(is_prime);
        return; /* cannot proceed without primes */
    }
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i) if (is_prime[i]) new_primes[pos++] = i;
    free(primes);
    primes = new_primes;
    primes_cnt = cnt;
    primes_limit = new_limit;
    free(is_prime);
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    ensure_primes_upto(n);
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) break;
        idx_t t = n / p;
        result += (seq_t)(i + 1) * (seq_t)t;
    }
    return result;
}

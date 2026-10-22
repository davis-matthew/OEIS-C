/*
 * A333172  a(n) = Sum_{k=0..n} sigma(k^2 + 1), where sigma(k) is the sum of
 *          divisors of k (A000203).
 * https://oeis.org/A333172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} sigma(k^2 + 1)
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 2099, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static int primes_initialized = 0;

static void ensure_primes(void)
{
    if (primes_initialized)
        return;
    /* limit for sieve: sqrt(100000001) ≈ 10000 + 1 */
    const idx_t limit = 10001;
    /* create a boolean array for sieve */
    char *is_comp = (char *)calloc(limit + 1, sizeof(char));
    size_t cap = 256;
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == cap) {
                cap <<= 1;
                primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_initialized = 1;
}

static seq_t sigma_uint64(uint64_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 1;
    uint64_t temp = n;
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint32_t p = primes[idx];
        if ((uint64_t)p * p > temp)
            break;
        if (temp % p == 0) {
            seq_t term = 1;
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= (seq_t)p;
                term += p_pow;          /* 1 + p + p^2 + ... */
            }
            result *= term;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        result *= (seq_t)(1 + temp);
    }
    return result;
}

seq_t a(idx_t n)
{
    ensure_primes();
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        uint64_t val = (uint64_t)k * (uint64_t)k + 1;
        sum += sigma_uint64(val);
    }
    return sum;
}

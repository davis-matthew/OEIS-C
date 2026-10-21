/*
 * A272171  Triangle read by rows: T(n,k) in which row n lists the first n
 *          terms of A000005 in reverse order.
 * https://oeis.org/A272171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = d(k) where k = m - (n - T(m-1)) + 1, m = smallest integer
 *            with T(m) >= n, T(m) = m(m+1)/2, and d(k) = A000005(k) (number
 *            of positive divisors of k)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small prime sieve used for divisor counting. */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;

    const size_t limit = 80000;              /* sqrt(6e9) ≈ 77459, safe bound */
    prime_list = (uint32_t *)malloc(limit * sizeof(uint32_t));
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!prime_list || !is_prime) {
        /* In a competitive environment, abort on allocation failure. */
        abort();
    }

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_list[prime_count++] = (uint32_t)i;
        }
    }

    free(is_prime);
}

/* Compute the number of divisors of n (n > 0). */
static seq_t divisor_count(uint64_t n)
{
    init_primes();
    seq_t result = 1;
    uint64_t x = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        if (x % p == 0) {
            size_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (x > 1) /* remaining prime factor */
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, return 0 */

    /* Find minimal m such that m(m+1)/2 >= n. */
    idx_t low = 0;
    idx_t high = (idx_t)1 << 33; /* 8,589,934,592 > sqrt(2*2^64) */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* T(m-1) = m(m-1)/2 */
    __int128 t_m1 = (__int128)m * (m - 1) / 2;
    idx_t t_m1_uint = (idx_t)t_m1;

    idx_t diff = n - t_m1_uint;          /* 1 <= diff <= m */
    idx_t k = m - diff + 1;              /* index into divisor sequence */

    /* Compute number of divisors of k. */
    return divisor_count((uint64_t)k);
}

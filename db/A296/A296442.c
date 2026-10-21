/*
 * A296442  Initial digit of n-th Mersenne number.
 * https://oeis.org/A296442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = leading decimal digit of the n-th Mersenne number where
 *            the n-th Mersenne number is 2^p_n - 1 and p_n is the n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

static const long double LOG10_2 = 0.3010299956639811952137389L;

/* Pre‑compute primes up to PRIME_LIMIT once. */
static void init_primes(uint32_t **prime_arr, size_t *prime_cnt)
{
    static bool initialized = false;
    static const int PRIME_LIMIT = 200000;   /* enough for n ≈ 1e4 */

    if (initialized) return;

    uint32_t *pr = (uint32_t*)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    bool *is_prime = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    memset(is_prime, 1, PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) pr[cnt++] = (uint32_t)i;

    free(is_prime);

    *prime_arr = pr;
    *prime_cnt = cnt;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined, but safe */

    /* Handle the two special cases where 2^p - 1 differs from 2^p. */
    if (n == 1) return (seq_t)3;          /* 2^2 - 1 = 3  */
    if (n == 2) return (seq_t)7;          /* 2^3 - 1 = 7  */

    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0;
    init_primes(&primes, &prime_cnt);

    if ((size_t)n > prime_cnt) return 0;  /* beyond pre‑computed range */

    idx_t p = primes[n - 1];               /* the n-th prime */

    long double f = fmodl((long double)p * LOG10_2, 1.0L);
    long double t = powl(10.0L, f);
    unsigned digit = (unsigned)floorl(t + 1e-12L);  /* safety epsilon */

    return (seq_t)digit;
}

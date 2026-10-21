/*
 * A292376  a(1) = 0, a(2n) = 0, and for odd numbers n > 1, a(n) =
 *          a(A064989(n)) + [n == 3 (mod 4)].
 * https://oeis.org/A292376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2n)=0, for odd n>1: a(n)=a(A064989(n))+[n≡3 (mod 4)]
 * Range:     u8 n <= 3606, u16 n <= 9553, u32 n <= 9553, u64 n <= 9553,
 *            u128 n <= 9553, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses prime factorisation and shifts every prime in the
   factorisation of an odd number to its immediate predecessor prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static bool primes_init = false;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_init) return;
    const int LIMIT = 60000;           /* sufficiently large for n < 1e5 */
    bool *sieve = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    memset(sieve, 0, (LIMIT + 1) * sizeof(bool));
    for (int i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (int j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;

    size_t cnt = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) primes[idx++] = (uint32_t)i;

    primes_cnt = cnt;
    primes_init = true;
    free(sieve);
}

/* return the immediate predecessor prime of a prime p (p>2) */
static uint64_t prev_prime_of(uint64_t p)
{
    /* binary search over the primes array */
    size_t lo = 1, hi = primes_cnt - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return (uint64_t)primes[mid - 1];
        if (primes[mid] < p)  lo = mid + 1;
        else                  hi = mid - 1;
    }
    /* Should never happen for our constraints */
    return 1;
}

/* compute A064989(n): replace every prime factor by its immediate
   predecessor prime.  n is guaranteed odd. */
static uint64_t map_prev_u64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t temp = n;
    uint64_t orig = temp;
    uint64_t limit = (uint64_t)sqrt((long double)orig) + 1;

    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            uint64_t prev = prev_prime_of(p);
            for (int j = 0; j < exp; ++j) res *= prev;
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        uint64_t prev = prev_prime_of(temp);
        res *= prev;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;
    init_primes();
    seq_t count = 0;

    while ((cur & 1ULL) && cur > 1ULL) {
        if ((cur % 4ULL) == 3ULL) count += 1;
        cur = map_prev_u64(cur);
    }
    return count;
}

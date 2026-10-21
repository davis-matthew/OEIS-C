/*
 * A260867  Least k > 1 that divides A260868(n) + 2^k - 2.
 * https://oeis.org/A260867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 that divides A260868(n) + 2^k – 2
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- helpers ----- */

/* fast modular exponentiation for uint64_t operands */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 0) return 0;               /* avoid division by 0 */
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* return the least prime factor of a uint64_t number;
   returns the number itself if it is prime          */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    if ((n & 1ULL) == 0ULL) return 2ULL;
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL)
        if (n % p == 0ULL)
            return p;
    return n;   /* n is prime */
}

/* least k > 1 such that k divides N + 2^k – 2 */
static uint64_t least_k(uint64_t N)
{
    for (uint64_t k = 2ULL; ; ++k)
    {
        uint64_t pow2 = mod_pow_uint64(2ULL, k, k);
        uint64_t val  = (N % k + pow2 + k - 2ULL) % k;
        if (val == 0ULL)
            return k;
    }
}

/* ----- sequence implementation ----- */

static seq_t *cached = NULL;   /* array of cached a(n) values */
static size_t count  = 0;      /* how many terms are cached so far */
static size_t capacity = 0;    /* size of the allocated array */
static uint64_t next_N = 2;    /* the next integer N to test */

seq_t a(idx_t n)
{
    /* ensure we have at least n terms cached */
    while (count < (size_t)n)
    {
        uint64_t N = next_N++;
        uint64_t lpf = smallest_prime_factor(N);
        uint64_t k   = least_k(N);

        if (k != lpf)   /* N belongs to A260868, record its k  */
        {
            if (count >= capacity)         /* grow cache if needed   */
            {
                capacity = capacity ? capacity << 1 : 16;
                void *ptr = realloc(cached, capacity * sizeof(seq_t));
                if (!ptr) exit(EXIT_FAILURE);
                cached = (seq_t *)ptr;
            }
            cached[count++] = (seq_t)k;
        }
    }
    return cached[(size_t)n - 1];
}

/*
 * A338690  Inverse Moebius transform of A209615.
 * https://oeis.org/A338690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A338690
   Inverse Möbius transform of A209615.
   Multiplicative with:
     a(p^e) = e + 1          if p ≡ 1 (mod 4)
     a(p^e) = (1 + (-1)^e)/2 if p = 2 or p ≡ 3 (mod 4)
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static prime cache ---------------------------------------------------- */
static uint64_t *g_primes = NULL;   /* prime list (value type uint64_t) */
static size_t   g_prime_cnt = 0;    /* number of primes stored */
static idx_t    g_prime_limit = 0;  /* largest sieved value */

/* Helper: compute integer sqrt of an idx_t (suitable for n≤UINT64_MAX).  */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Ensure all primes ≤ limit are stored in g_primes */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= g_prime_limit)
        return;                       /* already sufficient */

    /* free old list if any */
    free(g_primes);
    g_primes = NULL;
    g_prime_cnt = 0;
    g_prime_limit = limit;

    /* simple Eratosthenes sieve */
    bool *is_prime = (bool*)malloc((size_t)(limit + 1));
    if (!is_prime)
        exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    g_prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++g_prime_cnt;

    g_primes = (uint64_t*)malloc(g_prime_cnt * sizeof(uint64_t));
    if (!g_primes)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            g_primes[idx++] = (uint64_t)i;

    free(is_prime);
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset starts at 1, value for 0 is undefined */

    idx_t temp = n;
    ensure_primes_upto(isqrt_idx(temp));

    seq_t result = 1;
    for (size_t pi = 0; pi < g_prime_cnt && g_primes[pi] * g_primes[pi] <= temp; ++pi) {
        uint64_t p = g_primes[pi];
        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp == 0)
            continue;

        if (p == 2 || (p & 3) == 3) {            /* p = 2 or p ≡ 3 (mod 4) */
            if (exp & 1)                         /* odd exponent → zero */
                return (seq_t)0;
            /* even exponent → factor 1; nothing to multiply */
        } else {                                 /* p ≡ 1 (mod 4) */
            result *= (seq_t)(exp + 1);
        }
    }

    /* remaining prime factor (if any) > sqrt(original n) */
    if (temp > 1) {                       /* temp itself is prime */
        uint64_t p = temp;
        if (p == 2 || (p & 3) == 3)       /* odd exponent (1) → zero */
            return (seq_t)0;
        /* p ≡ 1 (mod 4) and exp = 1 → multiply by 2 */
        result *= (seq_t)2;
    }

    return result;
}

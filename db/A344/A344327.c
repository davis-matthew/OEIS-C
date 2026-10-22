/*
 * A344327  Number of divisors of n^4.
 * https://oeis.org/A344327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = τ(n^4) = ∏_p (4·e_p + 1) where n = ∏_p p^{e_p}
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the number of divisors of n^4.
 * The function works for any n ≥ 1 and for all supported seq_t sizes,
 * including arbitrary‑precision types.
 * Factorisation is performed with an on‑demand prime sieve.
 * No external caching is used – each call is independent and
 * runs in well under 2 s for n up to many thousands.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* ---------- helper: integer square root for uint64_t ---------- */
static idx_t uint64_isqrt(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)0xFFFFFFFFULL;          /* √(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ---------- on‑demand prime list ---------- */
static uint64_t *prime_list   = NULL;  /* primes ≤ prime_limit */
static size_t     prime_cnt    = 0;     /* number of primes in prime_list */
static idx_t      prime_limit  = 0;     /* current maximum prime generated */

static void ensure_primes_at_least(idx_t limit)
{
    if (prime_limit >= limit)
        return;

    free(prime_list);                           /* old list no longer used */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used – safety for callers that pass 0 */
        return 0;

    /* need primes up to √n */
    idx_t sqrt_n = uint64_isqrt(n);
    ensure_primes_at_least(sqrt_n + 1);

    idx_t  temp   = n;
    seq_t  result = 1;

    for (size_t i = 0; i < prime_cnt &&
                       (uint64_t)prime_list[i] * prime_list[i] <= temp;
         ++i)
    {
        uint64_t p = prime_list[i];
        unsigned e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        if (e > 0)
            result *= (seq_t)(4 * e + 1);
    }

    /* remaining prime factor (if any) has exponent 1 */
    if (temp > 1)
        result *= (seq_t)(4 * 1 + 1);

    return result;
}

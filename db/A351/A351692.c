/*
 * A351692  a(n) is the least number k such that 1 <= k < n and prime(n) +
 *          2*prime(n-k) and prime(n) + 2*prime(n+k) are both prime, or 0 if
 *          there is no such k.
 * https://oeis.org/A351692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive k (< n) such that
 *            prime(n)+2*prime(n-k) and prime(n)+2*prime(n+k) are both prime.
 *            If no such k exists a(n)=0. This implementation searches k from
 *            1 upward using a pre‑computed list of primes
 * Range:     u8 n <= 543, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

/* static storage for the sieve */
static uint64_t *prime_list   = NULL;      /* array of primes in ascending order (1‑based indexing) */
static size_t    prime_count  = 0;          /* number of primes stored */
static bool      sieve_ready  = false;

/* initialize the prime table to a bound that is large enough for
   n up to at least 10 000 (π(1 000 000)=78 498). */
static void init_primes(void)
{
    if (sieve_ready)
        return;
    const idx_t limit = 1000000;           /* safe upper bound for all requested n */
    build_primes_upto(limit, &prime_list, &prime_count);
    sieve_ready = true;
}

/* simple primality test using the pre‑computed prime list.
   Works for all integers that fit into seq_t (signedness not
   relevant, we treat them as unsigned). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return false;
    if (x == 2)
        return true;
    if ((x & 1) == 0)
        return false;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)               /* offset is 1; values below are undefined */
        return (seq_t)0;

    init_primes();

    /* safety: if the requested index is beyond our prime list, fail */
    if ((size_t)n > prime_count)
        return (seq_t)0;

    uint64_t p_n = prime_list[(size_t)n - 1];  /* nth prime (1‑based) */

    /* Search for the smallest k (1 <= k < n) satisfying the condition */
    for (idx_t k = 1; k < n; ++k) {
        size_t idx_minus = (size_t)n - k - 1;  /* n-k  (1‑based) -> 0‑based index */
        size_t idx_plus  = (size_t)n + k - 1;  /* n+k */

        if (idx_minus >= prime_count || idx_plus >= prime_count)
            break;

        uint64_t p_minus = prime_list[idx_minus];
        uint64_t p_plus  = prime_list[idx_plus];

        uint64_t A = p_n + 2ULL * p_plus;
        uint64_t B = p_n + 2ULL * p_minus;

        if (is_prime_uint64(A) && is_prime_uint64(B))
            return (seq_t)k;
    }

    return (seq_t)0;          /* no suitable k found */
}

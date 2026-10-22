/*
 * A376762  Number of composite numbers c in the range prime(n) < c <=
 *          2*prime(n+1).
 * https://oeis.org/A376762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n+1) - pi(2*prime(n+1)) - prime(n) + n
 * Range:     u8 n <= 60, u16 n <= 7100, u32 n <= 8570, u64 n <= 8570,
 *            u128 n <= 8570, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (prime(i) is the i‑th prime; pi(x) is the number of primes ≤ x) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime generation utilities (trial division) ---------- */

static uint32_t *prime_list = NULL;     /* array of found primes            */
static size_t   prime_len   = 0;         /* current number of primes stored */
static size_t   prime_cap   = 0;         /* allocated capacity of prime_list*/
static uint32_t next_candidate = 2;     /* next integer to test for primality */

/* Ensure that prime_list has capacity at least cap */
static void grow_prime_cap(size_t cap)
{
    if (prime_cap >= cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < cap) new_cap = cap;
    uint32_t *new_arr = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!new_arr) abort();               /* out of memory */
    prime_list = new_arr;
    prime_cap  = new_cap;
}

/* Append a new prime to prime_list */
static void add_prime(uint32_t p)
{
    grow_prime_cap(prime_len + 1);
    prime_list[prime_len++] = p;
}

/* Test whether n is prime using primes < sqrt(n) */
static bool is_prime_candidate(uint32_t n)
{
    for (size_t i = 0; i < prime_len; ++i) {
        uint32_t p = prime_list[i];
        if ((uint32_t)p * (uint32_t)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Extend prime_list so that every number ≤ limit has been tested.
   After the call prime_len contains the count of primes ≤ limit. */
static void ensure_primes_up_to_limit(uint32_t limit)
{
    while (next_candidate <= limit) {
        if (is_prime_candidate(next_candidate))
            add_prime((uint32_t)next_candidate);
        ++next_candidate;
    }
}

/* Ensure that prime_list has at least target_idx primes (1‑based index).
   If not, generate more primes until the count is reached. */
static void ensure_primes_count(size_t target_idx)
{
    while (prime_len < target_idx) {
        if (is_prime_candidate(next_candidate))
            add_prime((uint32_t)next_candidate);
        ++next_candidate;
    }
}

/* Binary search: number of primes ≤ x */
static size_t count_primes_le(uint32_t x)
{
    size_t lo = 0, hi = prime_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] <= x) lo = mid + 1;
        else hi = mid;
    }
    return lo;   /* lo is the count of primes ≤ x */
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1: undefined for n=0, return 0 */

    /* obtain primes needed for this n */
    ensure_primes_count((size_t)n + 1);        /* need prime(n+1) */
    uint32_t p = prime_list[(size_t)n - 1];    /* prime(n)   */
    uint32_t q = prime_list[(size_t)n];        /* prime(n+1) */

    /* need pi(2*q) : ensure primes up to 2*q are generated */
    uint32_t twoq = 2U * q;
    ensure_primes_up_to_limit(twoq);

    size_t pi_val = count_primes_le(twoq);

    seq_t result = (seq_t)2 * (seq_t)q - (seq_t)pi_val
                    - (seq_t)p + (seq_t)n;
    return result;
}

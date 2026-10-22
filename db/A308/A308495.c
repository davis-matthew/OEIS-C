/*
 * A308495  a(n) is the position of the first occurrence of prime(n) in
 *          A027748.
 * https://oeis.org/A308495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} floor(prime(n)/prime(k))
 * Range:     u8 n <= 34, u16 n <= 2846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

static void grow_prime_list(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (tmp) prime_list = tmp;
    prime_cap = new_cap;
}

static bool is_prime_candidate(uint64_t num)
{
    if (num < 2u) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        uint64_t sq = (uint64_t)p * (uint64_t)p;
        if (sq > num) break;
        if (num % p == 0) return false;
    }
    return true;
}

static void generate_primes(uint64_t needed)
{
    if (prime_count >= needed) return;
    uint64_t candidate = prime_count ? (uint64_t)prime_list[prime_count - 1] + 1u : 2u;
    while (prime_count < needed) {
        if (is_prime_candidate(candidate)) {
            if (prime_count == prime_cap) grow_prime_list();
            prime_list[prime_count++] = (uint32_t)candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* n should be >=1 as per OEIS offset */
    generate_primes(n);
    seq_t pn = (seq_t)prime_list[n - 1];
    seq_t sum = 1;
    for (idx_t i = 0u; i < n; ++i)
        sum += pn / (seq_t)prime_list[i];
    return sum;
}

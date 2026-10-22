/*
 * A372059  Indices of record high-points in A280741.
 * https://oeis.org/A372059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 5177, u32 n <= 5177, u64 n <= 5177,
 *            u128 n <= 5177, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A372059: indices of record high-points in A280741.
   The sequence is 1, 25, and all primes in increasing order.
   a(1) = 1, a(11) = 25, a(n) = the (n-1)-th prime for 2 ≤ n < 11,
   and a(n) = the (n-2)-th prime for n ≥ 12. The function generates
   primes on demand with trial division, keeping an internal list
   of primes which is expanded as needed.  This works for any
   seq_t/idx_t combination that satisfies the build constraints. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime cache */
static uint64_t *prime_list   = NULL;
static size_t  prime_cnt      = 0;   /* number of primes stored          */
static size_t  prime_cap      = 0;   /* capacity of prime_list            */

/* Append a prime to the cache, expanding the array if necessary */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap << 1 : 8;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

/* Generate primes until at least `need` primes are stored.
   Uses trial division by already found primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    uint64_t candidate = prime_cnt ? prime_list[prime_cnt - 1] + 1 : 2;
    while (prime_cnt < need) {
        bool is_prime = true;
        /* test divisibility by all stored primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > candidate / p) break;       /* p*p > candidate   */
            if (candidate % p == 0) {            /* divisible by p    */
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(candidate);
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* first term */

    if (n == 11) return (seq_t)25;        /* special term after 23 */

    if (n < 11) {                         /* primes 2..23 (indices 2-10) */
        size_t k = (size_t)(n - 1);       /* index in prime_list (1-based) */
        ensure_primes(k);
        return (seq_t)prime_list[k - 1];
    }

    /* n >= 12: term is the (n-2)-th prime */
    size_t k = (size_t)(n - 2);
    ensure_primes(k);
    return (seq_t)prime_list[k - 1];
}

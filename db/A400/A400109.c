/*
 * A400109  a(n) is the least prime p such that there is a prime q < p with
 *          gcd(q+1, p+1) = 2*n.
 * https://oeis.org/A400109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 772, u32 n <= 2506, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A400109:
   a(n) = least prime p such that there exists a prime q < p with
   gcd(q+1, p+1) = 2*n.
   The algorithm searches primes of the form p = 2*n*k - 1 and keeps
   indices k for which p is prime.  For each new prime, it checks if
   its index k is coprime to any previously found index; if so, the
   prime is the answer for the current n.  This uses only simple
   arithmetic and the provided is_prime_big and gcd utilities. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Euclidean algorithm on 64‑bit integers.  */
static inline uint64_t u64_gcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* d = 2*n, as seq_t */
    seq_t d = (seq_t)n * (seq_t)2;

    /* Dynamic array of indices k for which p = d*k - 1 is prime. */
    size_t idx_cap = 16;
    size_t idx_len = 0;
    uint64_t *idx_arr = (uint64_t *)malloc(idx_cap * sizeof(uint64_t));
    if (!idx_arr) abort();

    uint64_t k = 1;
    seq_t candidate;

    /* Find the first prime in the residue class. */
    for (;;) {
        candidate = d * (seq_t)k - (seq_t)1;
        if (candidate >= (seq_t)2 && is_prime_big(candidate)) {
            idx_arr[idx_len++] = k;
            break;
        }
        ++k;
    }

    /* Search for the minimal p satisfying the condition. */
    for (;;) {
        ++k;
        candidate = d * (seq_t)k - (seq_t)1;
        if (candidate < (seq_t)2) continue;
        if (!is_prime_big(candidate)) continue;

        /* Check gcd of k with previously found indices. */
        for (size_t i = 0; i < idx_len; ++i) {
            if (u64_gcd(k, idx_arr[i]) == 1) {
                free(idx_arr);
                return candidate;
            }
        }

        /* Store this index for future checks. */
        if (idx_len == idx_cap) {
            idx_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(idx_arr, idx_cap * sizeof(uint64_t));
            if (!tmp) { free(idx_arr); abort(); }
            idx_arr = tmp;
        }
        idx_arr[idx_len++] = k;
    }

    /* Unreachable but keeps the compiler happy. */
    free(idx_arr);
    return 0;
}

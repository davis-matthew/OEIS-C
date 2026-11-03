/*
 * A400109  a(n) is the least prime p such that there is a prime q < p with
 *          gcd(q+1, p+1) = 2*n.
 * https://oeis.org/A400109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 772, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9413 (2026-11-03)
 * Helpers:   is_prime_u64
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
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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

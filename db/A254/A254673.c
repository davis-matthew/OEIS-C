/*
 * A254673  Primes prime(n) such that prime(n) + 4*n is also prime.
 * https://oeis.org/A254673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth term is the nth prime p for which  p + 4*n  is also prime. */
/* Calls is_prime_small from the utils library to test primality. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* simple integer square root for uint64_t (used in trial division) */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ensure a dynamic array has enough capacity */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t newcap = (*cap ? *cap * 2 : 64);
        while (newcap < need) newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

static void grow_uint64_arr(uint64_t **arr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t newcap = (*cap ? *cap * 2 : 64);
        while (newcap < need) newcap <<= 1;
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
}

/* Return the nth term of A254673 (a(1) is the first term). */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1, but safe for n=0 */

    /* static state for memoization and prime generation */
    static seq_t  *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;

    /* initialise static arrays on first call */
    if (!seq) {
        seq = (seq_t *)malloc(64 * sizeof(seq_t));
        seq_cap = 64;
    }
    if (!primes) {
        primes = (uint64_t *)malloc(64 * sizeof(uint64_t));
        primes_cap = 64;
    }

    /* generate primes and build the sequence until we have enough terms */
    while (seq_len < (size_t)n) {

        /* --- find next prime --- */
        uint64_t candidate;
        if (primes_len == 0) {
            candidate = 2;
        } else {
            candidate = primes[primes_len - 1] + 1;
            if (candidate % 2 == 0) candidate++;   /* make odd */
        }

        for (;;) {
            uint64_t r = isqrt_u64(candidate);
            bool isprime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t p = primes[i];
                if (p > r) break;
                if (candidate % p == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime) break;         /* candidate is prime */
            candidate += 2;              /* next odd candidate */
        }

        /* store new prime */
        grow_uint64_arr(&primes, &primes_cap, primes_len + 1);
        primes[primes_len++] = candidate;

        /* prime index is 1‑based */
        idx_t prime_index = (idx_t)primes_len;

        /* test if candidate + 4*prime_index is prime */
        uint64_t q = candidate + 4 * (uint64_t)prime_index;
        if (is_prime_small(q)) {
            /* append to sequence */
            grow_seq_arr(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)candidate;
        }
    }

    return seq[n - 1];
}

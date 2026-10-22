/*
 * A306238  The smallest integer k such that floor(n!/k) is an odd number.
 * https://oeis.org/A306238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer k such that floor(n!/k) is odd
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fact_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/fact_mod_u64.h"

/* Allocate and cache primes up to a fixed bound. */
#define PRIME_BOUND 50000
static unsigned int primes[PRIME_BOUND / 10 + 100]; /* enough for bound */
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    unsigned char is_composite[PRIME_BOUND + 1];
    memset(is_composite, 0, sizeof(is_composite));
    for (unsigned int i = 2; i * i <= PRIME_BOUND; ++i) {
        if (!is_composite[i]) {
            for (unsigned int j = i * i; j <= PRIME_BOUND; j += i)
                is_composite[j] = 1;
        }
    }
    prime_cnt = 0;
    for (unsigned int i = 2; i <= PRIME_BOUND; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
        }
    }
}

/* The requested sequence function. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;
    if (n == 2 || n == 3) return 2;

    init_primes();

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        unsigned int p = primes[pi];
        uint64_t mod_val = (uint64_t)p * 2ULL;          /* 2p */
        uint64_t m_mod = fact_mod_u64(n, mod_val);     /* n! mod 2p */
        uint64_t r = m_mod % p;                        /* n! mod p */
        if (m_mod >= r + p) {                         /* floor is odd */
            return (seq_t)p;
        }
    }

    /* Should never reach here for n within typical ranges. */
    return (seq_t)1;
}

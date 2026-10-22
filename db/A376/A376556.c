/*
 * A376556  The number of non-unitary square divisors of binomial(n,
 *          floor(n/2)).
 * https://oeis.org/A376556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏(⌊e_i/2⌋+1) – 2^{#even e_i} * where e_i are the prime
 *            exponents of C(n,⌊n/2⌋)
 * Range:     u8 n <= 626, u16 n <= 6908, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/* Include helper that returns 2^exp as a seq_t */
#include "../utils/pow2.h"


/* Maximum N expected in tests – enough for n up to 20000.
 *  Adjust if needed. */
#define MAX_N 20000

static bool is_composite[MAX_N + 1];
static uint64_t primes[4000];
static size_t prime_count = 0;
static idx_t sieve_limit = 0;

static void sieve_upto(idx_t limit) {
    if (limit <= sieve_limit) return;
    if (limit > MAX_N) limit = MAX_N;  /* safety guard */

    /* Initialize the composite array */
    memset(is_composite, 0, sizeof(is_composite));

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_composite[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_composite[m] = true;
        }
    }

    /* Collect primes up to limit */
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) primes[prime_count++] = (uint64_t)i;
    }
    sieve_limit = limit;
}

seq_t a(idx_t n) {
    idx_t k = n / 2;          /* floor(n/2) */
    sieve_upto(n);

    seq_t total = 1;
    size_t even_cnt = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];

        /* Exponent of p in C(n,k) = exp(n!) - exp(k!) - exp((n-k)!)  */
        idx_t exp = 0;
        idx_t pp = p;
        while (pp <= n) {
            exp += n / pp;
            exp -= k / pp;
            exp -= (n - k) / pp;
            if (pp > n / p) break;   /* avoid overflow of pp * p */
            pp *= p;
        }

        if (exp == 0) continue;

        total *= (seq_t)(exp / 2 + 1);   /* (⌊e/2⌋+1) */
        if ((exp & 1) == 0) even_cnt++; /* e is even */
    }

    seq_t unitary = pow2((idx_t)even_cnt);   /* 2^{#even e_i} */
    return total - unitary;
}

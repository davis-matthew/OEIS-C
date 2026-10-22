/*
 * A352028  a(n) = Product p_{n*i}^e_i if the prime factorization of n is
 *          Product p_i^e_i.
 * https://oeis.org/A352028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 31, u64 n <= 127,
 *            u128 n <= 1023, big any n
 * Helpers:   sieve_check_bit, sieve_set_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Π p_{n*i}^{e_i} where n = Π p_i^{e_i} (OEIS A352028)
 * Computes the nth term for n ≥ 1.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/sieve_check_bit.h"
#include "../utils/sieve_set_bit.h"

/* Power with small exponent */
static seq_t pow_small(seq_t base, unsigned exp) {
    seq_t r = 1;
    for (unsigned i = 0; i < exp; ++i)
        r *= base;
    return r;
}

/* Global prime data (initialized once) */
static int  *prime_list    = NULL;          /* array of primes (1‑based index in OEIS) */
static size_t prime_count  = 0;             /* total number of primes generated */
static int   prime_idx[10001] = {0};        /* mapping from prime ≤ 10000 to its 1‑based index */
static bool   primes_ready = false;

/* Initialize primes up to the needed index for n ≤ 10000 */
static void init_primes(void) {
    if (primes_ready) return;
    /* Constants for n ≤ 10000 */
    const idx_t MAX_N = 10000;
    const idx_t PI_EST = 1300; /* safe upper bound for π(10000) = 1229 */

    idx_t target_index = MAX_N * PI_EST;         /* ~12,300,000 */
    /* Upper bound for the k‑th prime: p_k < k(ln k + ln ln k) for k ≥ 6 */
    int64_t limit;
    if (target_index < 6) {
        limit = 15;
    } else {
        double tk = (double)target_index;
        limit = (int64_t)(tk * (log(tk) + log(log(tk)))) + 3;
    }
    if (limit < 200) limit = 200;

    /* Allocate sieve (bitset) */
    size_t sieve_bytes = (limit >> 3) + 1;
    unsigned char *sieve = (unsigned char *)malloc(sieve_bytes);
    memset(sieve, 0, sieve_bytes);

    /* Mark even numbers >2 as composite */
    for (uint64_t i = 4; i <= (uint64_t)limit; i += 2) sieve_set_bit(sieve, i);

    /* Sieve odd numbers */
    uint64_t sqrt_limit = (uint64_t)sqrt((double)limit);
    for (uint64_t i = 3; i <= sqrt_limit; i += 2) {
        if (!sieve_check_bit(sieve, i)) {
            for (uint64_t j = i * i; j <= (uint64_t)limit; j += i << 1)
                sieve_set_bit(sieve, j);
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= (uint64_t)limit; ++i)
        if (!sieve_check_bit(sieve, i)) ++cnt;

    /* Allocate prime list */
    prime_list = (int *)malloc(cnt * sizeof(int));
    size_t idx = 0;
    for (uint64_t i = 2; i <= (uint64_t)limit; ++i) {
        if (!sieve_check_bit(sieve, i)) {
            prime_list[idx++] = (int)i;
        }
    }
    prime_count = cnt;

    /* Map small primes (≤ 10000) to their 1‑based index */
    for (int i = 1; i <= 10000; ++i) prime_idx[i] = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        int val = prime_list[i];
        if (val <= 10000) prime_idx[val] = (int)i + 1; /* 1‑based */
    }

    /* Clean up sieve */
    free(sieve);
    primes_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* out of range, but safe */

    init_primes();

    if (n == 1) return 1;

    idx_t m = n;
    seq_t result = 1;

    /* Factor n using the prime list (only small primes are needed) */
    for (size_t i = 0; i < prime_count; ++i) {
        int p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)m)
            break;
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* index of the factor prime */
            int index_of_p = prime_idx[p];
            /* compute prime at position n * index_of_p (1‑based) */
            idx_t target_pos = n * (idx_t)index_of_p;
            seq_t prime_at = (seq_t)prime_list[(size_t)(target_pos - 1)];
            result *= pow_small(prime_at, exp);
        }
    }

    /* Remaining part is a prime > sqrt(n) */
    if (m > 1) {
        int index_of_p = prime_idx[(int)m];
        idx_t target_pos = n * (idx_t)index_of_p;
        seq_t prime_at = (seq_t)prime_list[(size_t)(target_pos - 1)];
        result *= prime_at; /* exponent is 1 */
    }

    return result;
}

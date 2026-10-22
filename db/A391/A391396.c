/*
 * A391396  Numbers k such that k and k+1 are both numbers whose number of
 *          divisors is 3 times a power of 2 (A377562).
 * https://oeis.org/A391396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2886, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391396: numbers k such that k and k+1 have
 * divisor counts of the form 3·2^m (m≥0).  The function
 * searches forward, computing divisor counts by trial
 * division using a pre‑generated prime table.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/good_divisors.h"

/* ------------------------------------------------------------------ */
/* Prime table (sieve of Eratosthenes)                                 */
/* ------------------------------------------------------------------ */
static uint32_t primes[2000000];
static size_t   primes_cnt = 0;
static bool     primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    const size_t limit = 2000000;
    bool *is_composite = (bool*)malloc(limit + 1);
    memset(is_composite, 0, limit + 1);

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt < sizeof(primes)/sizeof(primes[0]))
                primes[primes_cnt++] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/* Divisor count                                                      */
static uint32_t divisor_count(uint64_t n)
{
    uint32_t res = 1;
    uint64_t temp = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp)
            break;
        if (temp % p == 0) {
            uint32_t exp = 1;
            temp /= p;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            res *= (exp + 1);
        }
    }
    if (temp > 1)
        res *= 2;   /* remaining prime factor */
    return res;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Sequence storage                                                   */
static seq_t *seq_storage = NULL;
static idx_t  seq_len     = 0;
static idx_t  seq_cap     = 0;

static void append_seq(uint64_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 8;
        seq_storage = (seq_t*)realloc(seq_storage, seq_cap * sizeof(seq_t));
    }
    seq_storage[seq_len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */
/* Main function                                                       */
seq_t a(idx_t n)
{
    static idx_t next_k = 1;   /* candidate integer to test */

    init_primes();            /* ensure prime list exists */

    while (seq_len < n) {
        /* Check current candidate and the following integer */
        uint64_t k = next_k;
        uint64_t k1 = k + 1;

        uint32_t d1 = divisor_count(k);
        uint32_t d2 = divisor_count(k1);

        if (good_divisors(d1) && good_divisors(d2))
            append_seq(k);

        ++next_k;
    }
    return seq_storage[n - 1];   /* vals fit in seq_t when needed */
}

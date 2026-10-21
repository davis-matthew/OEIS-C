/*
 * A296199  Numerator of the contraharmonic mean of the first n primes.
 * https://oeis.org/A296199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( Σ_{k=1..n} prime(k)^2 / Σ_{k=1..n} prime(k) )
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    uint32_t candidate = 2;
    if (prime_cnt > 0) {
        candidate = primes[prime_cnt - 1] + 1;
    }

    if (candidate != 2 && (candidate & 1u) == 0)
        candidate++;

    while (prime_cnt < (size_t)n) {
        bool is_prime = true;
        uint32_t limit = (uint32_t)sqrt((double)candidate);
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if (p > limit)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
                uint32_t *tmp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
                primes = tmp;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = candidate;
        }
        candidate += (candidate == 2) ? 1 : 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_primes(n);

    seq_t sum1 = 0;
    seq_t sum2 = 0;

    for (idx_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)primes[i];
        sum1 += p;
        sum2 += p * p;
    }

    seq_t g = gcd(sum2, sum1);
    return sum2 / g;
}

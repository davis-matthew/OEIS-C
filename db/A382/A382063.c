/*
 * A382063  Numbers whose number of coreful divisors is divisible by their
 *          number of exponential divisors.
 * https://oeis.org/A382063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_of_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382063: 
   a(n) = n‑th positive integer k such that
          coreful_divisor_count(k) % exponential_divisor_count(k) == 0.
   (coreful divisors: divisors sharing the radical of k; 
    exponential divisors: divisors whose prime exponents divide those of k) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/tau_of_uint.h"

/* Freezer for dynamic array growth */
static void *grow(void *p, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : *cap << 1;
        while (new_cap < need) new_cap <<= 1;
        p = realloc(p, new_cap * elem_size);
        *cap = new_cap;
    }
    return p;
}

/* Append a value to a dynamic array of seq_t values */
static void append_term(seq_t **buf, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *buf = (seq_t *)realloc(*buf, *cap * sizeof(seq_t));
    }
    (*buf)[(*len)++] = val;
}

/* Return the n-th term of A382063, n ≥ 1. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t candidate = 1;

    while (terms_len < n) {
        uint64_t m = candidate++;
        if (m == 1) {
            append_term(&terms, &terms_len, &terms_cap, (seq_t)1);
            continue;
        }

        /* Factor m into primes and exponents. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt = 0;
        uint64_t temp = m;

        /* factor 2 */
        if (temp % 2 == 0) {
            uint64_t cnt = 0;
            while (temp % 2 == 0) {
                temp /= 2;
                ++cnt;
            }
            primes[pcnt] = 2;
            exps[pcnt] = cnt;
            ++pcnt;
        }
        for (uint64_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                uint64_t cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++cnt;
                }
                primes[pcnt] = p;
                exps[pcnt] = cnt;
                ++pcnt;
            }
        }
        if (temp > 1) {
            primes[pcnt] = temp;
            exps[pcnt] = 1;
            ++pcnt;
        }

        /* coreful divisor count = product of exponents */
        seq_t coreful = 1;
        /* exponential divisor count = product of τ(e)  */
        seq_t expdiv = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            coreful *= (seq_t)exps[i];
            expdiv *= (seq_t)tau_of_uint(exps[i]);
        }

        if (expdiv != 0 && coreful % expdiv == 0)
            append_term(&terms, &terms_len, &terms_cap, (seq_t)m);
    }

    return terms[n - 1];
}

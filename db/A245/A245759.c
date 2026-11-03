/*
 * A245759  Primes p such that concatenation of p with its digit sum is also
 *          prime.
 * https://oeis.org/A245759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 8160, u64 n <= 8160,
 *            u128 n <= 8160, big any n
 * Validated: standard, pass, 65 terms through n = 7887 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A245759: primes p such that concatenation of p with its digit sum is also prime.
   The function Returns the n-th such prime (with a(1) = 61). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *terms = NULL;
    static size_t length = 0;   /* number of terms stored */
    static size_t capacity = 0; /* capacity of terms array */
    static uint64_t last_checked = 1; /* last integer examined for primality */

    if (n == 0)
        return 0; /* the sequence offset is 1 */

    /* generate terms until we have at least n */
    while (length < n) {
        /* find next prime after last_checked */
        last_checked = next_prime(last_checked + 1);
        seq_t prime = (seq_t)last_checked;

        /* digit sum of the prime */
        seq_t dsum = digit_sum(prime);

        /* concatenation of prime and its digit sum */
        seq_t concat = concat_parts((idx_t)prime, (idx_t)dsum);

        /* check if the concatenation is prime */
        if (is_prime_big(concat)) {
            /* grow cache if necessary */
            if (length == capacity) {
                size_t old_cap = capacity;
                capacity = capacity ? capacity << 1 : 16;
                seq_t *tmp = seq_realloc(terms, old_cap, capacity);
                terms = tmp;
            }
            terms[length++] = prime;
        }
    }

    return terms[n - 1];
}

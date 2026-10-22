/*
 * A347826  Numbers k with at least one odd semiprime divisor < k.
 * https://oeis.org/A347826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n that have an odd semiprime divisor d < n. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Simple primality test for small numbers. */
static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t p = 3; p * p <= x; p += 2)
        if (x % p == 0) return false;
    return true;
}

/* Return true iff n has an odd semiprime divisor smaller than n. */
static bool has_semiprime_divisor(uint64_t n)
{
    if (n <= 1) return false;

    /* odd part of n */
    uint64_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;
    if (odd == 1) return false;            /* n is a power of 2 */

    if ((n & 1) == 0) {                    /* n is even */
        /* For an even n, d must be the odd part of n. */
        return !is_prime_small(odd);       /* odd part composite */
    }

    /* n is odd: factor its odd part (which is n itself). */
    uint64_t m = odd;
    int exp[64];
    int exp_cnt = 0;
    int distinct = 0;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            exp[exp_cnt++] = e;
            ++distinct;
        }
    }
    if (m > 1) {               /* remaining prime factor */
        exp[exp_cnt++] = 1;
        ++distinct;
    }

    if (distinct == 1) {
        /* n = p^e */
        if (exp[0] == 1)        return false;      /* prime */
        if (exp[0] == 2)        return false;      /* p^2 itself, no proper semiprime divisor */
        return true;                                           /* p^e with e>2 gives p^2 as divisor */
    } else if (distinct == 2) {
        if (exp[0] == 1 && exp[1] == 1) return false; /* n = p*q, no proper semiprime divisor */
        return true;                                           /* at least one exponent >=2 gives a semiprime divisor */
    }
    /* distinct >= 3 : always has a semiprime divisor (product of two primes). */
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;
    static idx_t  seq_cap = 0;
    static idx_t  seq_len = 0;
    static uint64_t current = 17;   /* next candidate will be current+1 */

    if (n == 0) return 0;  /* safe guard, seq starts at index 1 */

    while (seq_len < n) {
        ++current;
        if (has_semiprime_divisor(current)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) { free(seq); abort(); }
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)current;
        }
    }
    return seq[n - 1];
}

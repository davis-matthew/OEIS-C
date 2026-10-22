/*
 * A395788  Numbers k for which sopfr(2*k) = sopfr(k + 2) where sopfr =
 *          A001414.
 * https://oeis.org/A395788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 895, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that sopfr(2·k) == sopfr(k + 2). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Sum of prime factors with multiplicity for 64‑bit unsigned integers. */
static seq_t sum_prime_factors_uint64(uint64_t n)
{
    seq_t sum = 0;
    while ((n & 1ULL) == 0ULL) {
        sum += (seq_t)2;
        n >>= 1;
    }
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        while (n % p == 0ULL) {
            sum += (seq_t)p;
            n /= p;
        }
    }
    if (n > 1ULL) {
        sum += (seq_t)n;
    }
    return sum;
}

/* Static data for memoization. */
static seq_t *terms     = NULL;   /* array of qualifying k values */
static size_t terms_len = 0;       /* number of stored terms */
static size_t terms_cap = 0;       /* allocated capacity */
static uint64_t next_k   = 1;      /* next candidate k to test */

/* Ensure sufficient storage capacity. */
static void terms_ensure_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory */
    terms = tmp;
    terms_cap = new_cap;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside the offset, unused */

    size_t need = (size_t)n;
    while (terms_len < need) {
        uint64_t k = next_k;
        seq_t s2k = sum_prime_factors_uint64(k << 1);  /* sopfr(2*k) */
        seq_t sk2 = sum_prime_factors_uint64(k + 2);    /* sopfr(k+2) */
        if (s2k == sk2) {
            terms_ensure_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)k;
        }
        ++next_k;  /* advance candidate */
    }
    return terms[need - 1];
}

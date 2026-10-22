/*
 * A346695  Numbers with more divisors than digits in their binary
 *          representation.
 * https://oeis.org/A346695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with more divisors than digits in their binary representation.
   a(1) = 6, a(2) = 12, … */
/* Implemented by generating terms until the requested index is reached. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: count the number of divisors of a 64‑bit integer. */
static uint32_t divisor_count_u64(uint64_t n)
{
    if (n == 1ULL)
        return 1U;
    uint32_t dcnt = 1U;
    uint64_t tmp = n;

    for (uint64_t p = 2ULL; p * p <= tmp; ++p)
    {
        if (tmp % p == 0ULL)
        {
            uint32_t exp = 0U;
            while (tmp % p == 0ULL)
            {
                tmp /= p;
                ++exp;
            }
            dcnt *= (exp + 1U);
        }
    }
    if (tmp > 1ULL)
        dcnt *= 2U;

    return dcnt;
}

/* Helper: number of bits in the binary representation of x (x >= 1). */
static unsigned bitlen_uint64(uint64_t x)
{
    return x == 0ULL ? 0U : 64U - __builtin_clzll(x);
}

/* Static cache of generated terms. */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;
static uint64_t current_candidate = 2ULL;

/* Append a term to the cache, expanding the array when necessary. */
static void append_term(seq_t val)
{
    if (terms_cnt >= terms_cap)
    {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_cnt++] = val;
}

/* Generate terms until at least n terms are stored. */
static void ensure_terms(idx_t n)
{
    while (terms_cnt < n)
    {
        uint64_t v = current_candidate;
        uint32_t dcnt = divisor_count_u64(v);
        unsigned bcnt = bitlen_uint64(v);
        if (dcnt > bcnt)
            append_term((seq_t)v);
        ++current_candidate;
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0; /* undefined for indices < 1 */
    ensure_terms(n);
    return terms[n - 1];
}

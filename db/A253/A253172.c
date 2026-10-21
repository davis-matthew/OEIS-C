/*
 * A253172  Numbers n = p * q, where n, p, and q together contain all 10
 *          digits at least once.
 * https://oeis.org/A253172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A253172: a(n) is the n-th integer n such that n = p * q
    and the decimal digits of n, p, and q together contain all 10
    digits 0–9.  The implementation searches consecutive integers
    and tests factors until a match is found. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_small.h"

/* Bit mask for all ten digits */
#define ALL_DIGIT_MASK 0x3FFU

/* ----- Helper: digit mask ------------------------------------------ */
static inline unsigned digit_mask(seq_t x)
{
    unsigned m = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        m |= 1u << d;
        x /= 10;
    }
    return m;
}

/* ----- Check if a given number satisfies the condition ----------- */
static bool is_valid_term(seq_t m)
{
    unsigned mask_m = digit_mask(m);

    /* Quick accept if m itself is pandigital. */
    if (mask_m == ALL_DIGIT_MASK)
        return true;

    /* If m fits in 64‑bit, try to skip full divisor loop for primes. */
    if (m <= UINT64_MAX) {
        if (is_prime_small((uint64_t)m)) {
            /* Only factorization candidate is 1 * m. */
            if ((mask_m | 0x2U) == ALL_DIGIT_MASK)   /* mask of digit 1 */
                return true;
            return false;
        }
    }

    /* General divisor search. */
    idx_t sqrt_m = (idx_t)floor_sqrt(m);

    for (idx_t p = 1; p <= sqrt_m; ++p) {
        if ((m % p) != 0)
            continue;

        unsigned mask_p = digit_mask((seq_t)p);
        if ((mask_m | mask_p) == ALL_DIGIT_MASK)
            return true;                      /* p and m already cover all digits */

        seq_t q = m / p;
        unsigned mask_q = digit_mask(q);
        if ((mask_m | mask_p | mask_q) == ALL_DIGIT_MASK)
            return true;
    }
    return false;
}

/* ----- Memoization storage ------------------------------------------ */
static seq_t *seq = NULL;          /* array of found terms */
static idx_t term_count = 0;       /* how many terms stored */
static idx_t term_cap   = 0;       /* allocated capacity */
static seq_t next_candidate = 2;   /* next integer to test */

/* Resize storage if needed. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= term_cap)
        return;
    idx_t new_cap = term_cap ? term_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq) {
        memcpy(new_arr, seq, term_count * sizeof(seq_t));
        free(seq);
    }
    seq = new_arr;
    term_cap = new_cap;
}

/* ----- Sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined, but safe */

    /* Generate terms until we have at least n elements. */
    while (term_count < n) {
        seq_t m = next_candidate++;
        if (is_valid_term(m)) {
            ensure_capacity(term_count + 1);
            seq[term_count++] = m;
        }
    }
    return seq[n - 1];
}

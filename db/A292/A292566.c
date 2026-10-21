/*
 * A292566  Primes that can be reached with their prime-index, if you start
 *          with the prime-index and use iterations of A062028.
 * https://oeis.org/A292566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292566: Primes that can be reached from their prime index by
 * iterating f(x) = x + (sum of decimal digits of x).
 * The n‑th term a(n) (n ≥ 1) is returned.  No hard‑coded values. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t n); */
#include "../utils/digit_sum.h"    /* seq_t digit_sum(seq_t); */

/* Check whether the prime p can be obtained from its index k by repeated
 * application of f(x)=x+digit_sum(x).  The loop stops once the value
 * reaches or exceeds p. */
static bool qualifies(seq_t p, idx_t k)
{
    seq_t x = (seq_t)k;
    while (x < p)
        x += digit_sum(x);
    return x == p;
}

/* Lazy, memoised generation of the sequence.  We keep a dynamic array
 * of the qualifying primes and a running counter of the next prime to
 * be examined. */
static seq_t *sequence = NULL;
static size_t seq_len  = 0, seq_cap = 0;
static idx_t next_candidate  = 2;   /* next integer to test for primality   */
static idx_t next_prime_idx  = 0;   /* 1‑based index of the next prime */

/* Grow the sequence buffer to accommodate at least new_len elements. */
static void grow_sequence(size_t new_len)
{
    if (new_len <= seq_cap)
        return;
    size_t cap = seq_cap ? seq_cap * 2 : 16;
    while (cap < new_len)
        cap *= 2;
    sequence = (seq_t *)realloc(sequence, cap * sizeof(seq_t));
    seq_cap = cap;
}

/* Ensure that the sequence contains at least n terms (n ≥ 1). */
static void ensure_sequence(idx_t n)
{
    while (seq_len < n) {
        /* Find next prime */
        while (!is_prime(next_candidate))
            ++next_candidate;
        ++next_prime_idx;                /* index of this prime (1‑based) */

        seq_t p = (seq_t)next_candidate;
        if (qualifies(p, next_prime_idx)) {
            grow_sequence(seq_len + 1);
            sequence[seq_len++] = p;
        }

        ++next_candidate;   /* advance to the next integer for the next loop */
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n = 0; return 0 gracefully */
        return 0;
    ensure_sequence(n);
    return sequence[n - 1];
}

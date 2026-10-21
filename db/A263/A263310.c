/*
 * A263310  Numbers n such that p=6*n+1, q=6*p+1 and r=6*q+1 are primes.
 * https://oeis.org/A263310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer n such that * p = 6*n + 1, q = 6*p + 1,
 *            r = 6*q + 1 are all primes
 * Range:     u8 none, u16 n <= 7, u32 n <= 9, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates terms on demand and memoises the
 * sequence.  It uses deterministic Miller-Rabin for 64‑bit
 * prime testing via is_prime_big. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *sequence = NULL;     /* memoised terms */
static idx_t  count    = 0;         /* number of terms memoised   */
static idx_t  next_min = 1;         /* next candidate to test     */
static size_t capacity = 0;         /* allocated size of sequence  */

/* Append a value to the memoised array, growing it as needed. */
static void append_term(seq_t val)
{
    if (count >= capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        seq_t *new_seq = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
        if (!new_seq) abort();        /* heap exhaustion */
        sequence = new_seq;
        capacity = new_cap;
    }
    sequence[count++] = val;
}

seq_t a(idx_t n)
{
    /* Offset 1: the first term is a(1).  n==0 is undefined. */
    if (n == 0) return (seq_t)0;

    /* Generate terms until we have at least n terms. */
    while (count < n) {
        seq_t cand = (seq_t)next_min;          /* candidate n            */
        seq_t p    = 6 * cand + 1;             /* p = 6*n + 1           */
        seq_t q    = 6 * p   + 1;              /* q = 6*p + 1           */
        seq_t r    = 6 * q   + 1;              /* r = 6*q + 1           */

        if (is_prime_big(p) && is_prime_big(q) && is_prime_big(r))
            append_term(cand);

        ++next_min;                            /* next candidate to test */
    }

    return sequence[n - 1];
}

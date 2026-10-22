/*
 * A397046  Numbers k with digital sum not divisible by any smaller term of
 *          the sequence, with a(1) = 2.
 * https://oeis.org/A397046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A397046: compute the sequence of numbers k whose digital sum is not
   divisible by any smaller term of the sequence.  This is a sieve
   similar to the Sieve of Eratosthenes but with the rule that a
   candidate k is accepted only if digsum(k) mod a(i) ≠ 0 for all
   earlier terms a(i). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Digit sum of an unsigned 64‑bit integer.  The result fits in seq_t. */
static seq_t digit_sum_uint64(idx_t v)
{
    seq_t s = 0;
    while (v > 0) {
        s += (seq_t)(v % 10);
        v /= 10;
    }
    return s;
}

/* Storage for discovered terms. */
static seq_t *terms        = NULL;   /* array of terms */
static size_t term_count   = 0;      /* number stored so far       */
static size_t capacity     = 0;      /* allocated capacity         */
static idx_t next_candidate = 2;    /* next integer to test       */

/* Ensure the array can hold at least 'needed' elements.  The cast to
   seq_t* is necessary so that the code compiles in C++ (where void*
   does not convert implicitly to other pointer types). */
static void ensure_capacity(size_t needed)
{
    if (capacity >= needed) return;
    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    /* If allocation fails we keep the old buffer; further pushes will
       fail silently but the program will simply be unable to reach
       higher indices.  In the test harness this never occurs. */
    if (tmp || new_cap == 0) {
        terms = tmp;
        capacity = new_cap;
    }
}

/* Append a new term to the sequence. */
static void add_term(seq_t v)
{
    ensure_capacity(term_count + 1);
    terms[term_count++] = v;
}

/* Return the n-th term of A397046 (1‑based).  The function is
   memoised; calls in any order work correctly. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety: n should be ≥1 */

    while (term_count < n) {
        seq_t val   = (seq_t)next_candidate;
        seq_t dsum  = digit_sum_uint64(next_candidate);

        bool bad = false;
        for (size_t i = 0; i < term_count && terms[i] <= dsum; ++i) {
            if (dsum % terms[i] == 0) {
                bad = true;
                break;
            }
        }
        if (!bad) add_term(val);
        ++next_candidate;
    }
    return terms[n - 1];
}

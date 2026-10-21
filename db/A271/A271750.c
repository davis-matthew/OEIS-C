/*
 * A271750  a(1) = 1; a(n+1) = smallest Fibonacci number > a(n) with leading
 *          digit equal to the final digit of a(n).
 * https://oeis.org/A271750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n+1) = smallest Fibonacci number > a(n) whose leading decimal digit
   equals the last decimal digit of a(n). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic cache of known terms. */
static seq_t *terms = NULL;   /* array of computed terms */
static size_t term_len = 0;   /* number of terms stored */
static size_t term_cap = 0;   /* allocated capacity */

/* Fibonacci generator state:
   fib_prev  = F(k-1), fib_curr = F(k)  (k is fib_idx). */
static idx_t fib_idx = 1;   /* current Fibonacci index, F(1)=1 */
static seq_t fib_prev = 0;  /* F(0) */
static seq_t fib_curr = 1;  /* F(1) */

/* ------------------------------------------------------------------ */
/* Ensure that the terms cache has at least needed slots. */
static void ensure_terms_cap(size_t needed)
{
    if (term_cap < needed) {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        term_cap = new_cap;
    }
}

/* Return the first decimal digit of a non‑negative seq_t value. */
static int leading_digit(seq_t x)
{
    while (x >= 10)
        x /= 10;
    return (int)x;
}

/* Advance the Fibonacci generator until fib_curr > limit. */
static void advance_fib_until(seq_t limit)
{
    while (fib_curr <= limit) {
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
        fib_idx++;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence routine. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n < 1 */

    /* Initialise cache if this is the first call. */
    if (terms == NULL) {
        term_cap = 16;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        term_len = 0;
    }

    /* The very first term is a(1)=1. */
    if (term_len == 0) {
        ensure_terms_cap(1);
        terms[0] = 1;
        term_len = 1;
    }

    /* If the requested term is already known, return it. */
    if ((size_t)n <= term_len)
        return terms[n - 1];

    /* Compute all missing terms up to n. */
    for (size_t i = term_len; i < (size_t)n; ++i) {
        seq_t prev = terms[i - 1];
        int last_digit = (int)(prev % 10);           /* last decimal digit of a(n) */

        /* Find the smallest Fibonacci > prev. */
        advance_fib_until(prev);

        /* Now fib_curr is the first Fibonacci > prev. */
        /* If its leading digit matches, we are done. */
        while (leading_digit(fib_curr) != last_digit) {
            /* Advance to the next Fibonacci number. */
            seq_t next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = next;
            fib_idx++;
        }

        /* Store the found term. */
        ensure_terms_cap(i + 1);
        terms[i] = fib_curr;
        term_len = i + 1;

        /* Prepare for the next iteration: start searching from the next Fibonacci. */
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
        fib_idx++;
    }

    return terms[n - 1];
}

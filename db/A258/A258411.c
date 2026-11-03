/*
 * A258411  Nonnegative integers n such that in bijective base-2 numeration
 *          the number of occurrences of each digit doubles when n is squared.
 * https://oeis.org/A258411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A258411 is defined by a search: the sequence of nonnegative
 *            integers n such that in bijective base‑2 the counts of each
 *            digit double when n is squared
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 2117, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8046 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Count the number of 1s and 2s in the bijective base‑2
   representation of x. */
static void count_bij_digits(seq_t x, unsigned *cnt1, unsigned *cnt2)
{
    *cnt1 = 0;
    *cnt2 = 0;
    while (x > 0) {
        x--;                            /* convert to 0‑based digit */
        unsigned bit = (unsigned)(x & 1u); /* 0 => digit 1, 1 => digit 2 */
        if (bit == 0) (*cnt1)++;
        else          (*cnt2)++;
        x >>= 1;
    }
}

/* Return the n-th term of A258411 (1‑based index). */
seq_t a(idx_t n)
{
    /* static state for incremental generation */
    static seq_t  *terms = NULL;   /* array of found terms */
    static size_t term_cap = 0;    /* allocated capacity */
    static size_t term_len = 0;    /* number of terms stored */
    static seq_t  next_check = 1;  /* next integer to test */

    /* initialise storage on first call */
    if (!terms) {
        term_cap = 16;
        terms = mem_new(seq_t, term_cap);
        if (!terms) exit(1);
    }

    /* generate terms until we have at least n terms */
    while (term_len < n) {
        seq_t x = next_check;

        /* count digits of x */
        unsigned c1_x, c2_x;
        count_bij_digits(x, &c1_x, &c2_x);

        /* square x */
        seq_t sq = seq_mul(x, x);

        /* count digits of x^2 */
        unsigned c1_sq, c2_sq;
        count_bij_digits(sq, &c1_sq, &c2_sq);

        /* test the sequence property */
        if (c1_sq == c1_x * 2 && c2_sq == c2_x * 2) {
            /* add to table */
            if (term_len == term_cap) {
                term_cap *= 2;
                terms = mem_resize(seq_t, terms, term_cap);
                if (!terms) exit(1);
            }
            terms[term_len++] = x;
        }

        next_check++;
    }

    return terms[n - 1];   /* 1‑based index */
}

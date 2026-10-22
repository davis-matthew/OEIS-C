/*
 * A365822  List the positive integers but erase commas between terms if the
 *          digits just before and just after the comma have the same parity.
 * https://oeis.org/A365822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 1503, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   first_digit_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Sequence A365822  
   Generate the natural numbers in order.  
   Merge an adjacent pair (i , j) into one term iff  
   the last digit of i and the first digit of j have the same parity.  
   Each merge concatenates the two numbers; when a pair does not merge,
   the current group is finished and stored as a sequence term.  
   The function a(n) returns the n-th term of this sequence. */

/* Assumes seq_t and idx_t are defined elsewhere in the build. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/first_digit_idx.h"

/* Helper: number of decimal digits of a natural number (idx_t). */
static int digit_count_idx(idx_t n)
{
    int c = 0;
    do {
        ++c;
        n /= 10;
    } while (n);
    return c;               /* at least 1 */
}

/* Helper: power of 10, computed for a small exponent. */
static seq_t pow10_digits(int d)
{
    seq_t res = 1;
    for (int i = 0; i < d; ++i)
        res *= (seq_t)10;
    return res;
}

/* Dynamic array of seq_t terms, built incrementally. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Current state of the group being built. */
static seq_t current_val = 0;       /* concatenated value of the current group */
static int current_last_digit = 0;  /* last decimal digit of current group */
static idx_t next_num = 2;          /* next natural number to be considered */

/* Ensure the terms array has enough capacity */
static void ensure_capacity(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In practice this should not happen in the test harness. */
        abort();
    }
    terms = tmp;
    terms_cap = new_cap;
}

/* Append a new term to the sequence. */
static void append_term(seq_t val)
{
    ensure_capacity(terms_len + 1);
    terms[terms_len++] = val;
}

/* Public function adherent to required signature. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 1, but guard anyway */

    /* Initialise static state on first call. */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        /* First group starts with number 1. */
        current_val = (seq_t)1;
        current_last_digit = 1 % 10;
        next_num = 2;
    }

    /* Generate terms until we have at least n terms. */
    while (terms_len < (size_t)n) {
        int first_digit_next = first_digit_idx(next_num);
        int parity_last = current_last_digit & 1;
        int parity_first = first_digit_next & 1;

        if (parity_last == parity_first) {
            /* Merge current group with next_num. */
            int d = digit_count_idx(next_num);
            seq_t mult = pow10_digits(d);
            current_val = current_val * mult + (seq_t)next_num;
            current_last_digit = (int)(next_num % 10);
        } else {
            /* Finish current group. */
            append_term(current_val);
            /* Start new group with next_num. */
            current_val = (seq_t)next_num;
            current_last_digit = (int)(next_num % 10);
        }
        ++next_num;
    }

    return terms[n - 1];
}

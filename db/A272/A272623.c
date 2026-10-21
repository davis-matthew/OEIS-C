/*
 * A272623  Fibonacci numbers whose first digit is equal to its last digit.
 * https://oeis.org/A272623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the OEIS sequence A272623.
   It searches the Fibonacci numbers F_k.  Whenever the first
   decimal digit of F_k equals its last digit, F_k is output.
   The index n is 1‑based; a(1) = 0, a(2) = 1, a(3) = 2, ... */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>

/* Hard‑coded constants for logarithmic estimation of first digit of Fibonacci numbers.
   They are used only for very large indices where F_k cannot be represented in
   native arithmetic.  The constants have sufficient precision for the
   required range. */
#define LOG10_PHI   0.20898764024997873   /* log10((1+sqrt(5))/2) */
#define LOG10_SQRT5 0.3494850021680094    /* log10(sqrt(5)) */

/* Cache of Fibonacci numbers that satisfy the property.
   The array is grown when needed.  The cache can be visited
   in any order – the routine is fully memoised. */
static seq_t *found = NULL;
static size_t found_cap = 0;
static idx_t  found_cnt = 0;

/* State of a single pass through the Fibonacci sequence.
   The values are stored for fast iteration. */
static seq_t f_prev = 0, f_curr = 1; /* F_(k-1), F_k */
static int   d_prev = 0, d_curr = 1; /* F_(k-1) mod 10, F_k mod 10 */
static idx_t fib_index = 1;          /* index of f_curr (currently 1) */
static bool  initialized = false;

/* Append a new value to the cache, reallocating as required. */
static void add_value(seq_t val)
{
    if (found_cnt == found_cap) {
        size_t new_cap = (found_cap == 0) ? 16 : found_cap * 2;
        /* explicit cast for both C and C++ */
        seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);       /* memory exhaustion – abort */
        found = tmp;
        found_cap = new_cap;
    }
    found[found_cnt++] = val;
}

/* Return the nth term (1‑based) of the sequence.  The routine is
   fully memoised – calls may come in any order. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* behaviour for n<1 is not required */

    if (!initialized) {
        /* initialise cache with the first two Fibonacci numbers
           that satisfy the property: 0 and 1. */
        add_value((seq_t)0);          /* F_0 */
        add_value((seq_t)1);          /* F_1 */
        initialized = true;
    }

    /* Produce Fibonacci numbers until the cache contains at least n entries. */
    while (found_cnt < n) {
        /* Compute next Fibonacci number and its last digit. */
        seq_t f_next = f_prev + f_curr;
        int   d_next = (d_prev + d_curr) % 10;
        idx_t i_next = fib_index + 1;           /* index of f_next */

        /* Determine the first decimal digit of f_next.  For very large
           indices we use the logarithmic approximation:  F_k ≈ φ^k/√5. */
        int first_digit;
        if (f_next < (seq_t)10) {
            /* f_next is small – it fits in native integer. */
            first_digit = (int)(unsigned long long)(f_next);
        } else {
            double tmp = (double)i_next * LOG10_PHI - LOG10_SQRT5;
            double frac = tmp - floor(tmp);            /* fractional part */
            double pow10frac = pow(10.0, frac);        /* between 1 and 10 */
            first_digit = (int)(pow10frac + 1e-12);    /* guard against 9.999… */
        }

        /* If the first digit equals the last digit, store the value
           (ensuring it is unique in the cache). */
        if (first_digit == d_next) {
            if (found_cnt == 0 || f_next != found[found_cnt-1])
                add_value(f_next);
        }

        /* Advance to the next Fibonacci number. */
        f_prev = f_curr;   f_curr = f_next;
        d_prev = d_curr;   d_curr = d_next;
        fib_index = i_next;
    }

    return found[n-1];   /* cache is 0‑based, user index is 1‑based */
}

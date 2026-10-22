/*
 * A322892  a(n) = [x^n] Product_{k=1..n} (k + x + 2*k*x^2), for n >= 0.
 * https://oeis.org/A322892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k=1..n}(k + x + 2*k*x²) (offset 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static working buffers.  They are sized to 2*max_n + 1, where
   max_n is the largest n requested so far.  The buffers are
   swapped between iterations so that coeff always points to
   the current polynomial coefficients.  */
static seq_t *coeff = NULL;          /* current polynomial coefficients      */
static seq_t *temp  = NULL;          /* work buffer for the next iteration  */
static size_t  cap  = 0;             /* current allocated capacity (size)   */
static idx_t   current = 0;          /* largest n that has been processed   */

seq_t a(idx_t n)
{
    /* Ensure allocated capacity is at least 2*n + 1 coefficients. */
    if (2 * (size_t)n + 1 > cap) {
        size_t new_cap = 2 * (size_t)n + 1;
        seq_t *new_coeff = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
        seq_t *new_temp  = (seq_t *)realloc(temp,  new_cap * sizeof(seq_t));
        coeff = new_coeff;
        temp  = new_temp;
        /* Zero any newly allocated space. */
        if (new_cap > cap) {
            memset(&coeff[cap], 0, (new_cap - cap) * sizeof(seq_t));
            memset(&temp[cap],  0, (new_cap - cap) * sizeof(seq_t));
        }
        cap = new_cap;
    }

    /* Initialise for n = 0.  coeff[0] = 1 and all others stay zero. */
    if (current == 0 && coeff[0] == 0) {
        coeff[0] = 1;
    }

    /* Incrementally extend the product up to the requested n. */
    for (size_t k = current + 1; k <= (size_t)n; ++k) {
        seq_t k_seq      = (seq_t)k;
        seq_t two_k_seq  = k_seq + k_seq;           /* 2*k as seq_t        */
        size_t old_deg   = 2 * (k - 1);             /* previous degree      */
        size_t new_deg   = 2 * k;                   /* new degree after step */

        /* Zero the work buffer up to the new degree. */
        memset(temp, 0, (new_deg + 1) * sizeof(seq_t));

        /* Polynomial multiplication: (coeff) * (k + x + 2*k*x²). */
        for (size_t i = 0; i <= old_deg; ++i) {
            seq_t val = coeff[i];
            if (val == 0) continue;                /* skip zeros if desired */
            temp[i]     += val * k_seq;            /* multiply by k        */
            temp[i + 1] += val;                    /* multiply by 1        */
            temp[i + 2] += val * two_k_seq;        /* multiply by 2k       */
        }

        /* Swap buffers for the next iteration. */
        seq_t *swap = coeff;
        coeff = temp;
        temp = swap;

        current = k;
    }

    return coeff[n];
}

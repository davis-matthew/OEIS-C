/*
 * A306510  Numbers k such that twice the number of divisors of k is equal to
 *          the number of divisors of the sum of digits of k.
 * https://oeis.org/A306510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2579, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306510: Numbers k such that 2*divisor_count(k)=divisor_count(sum_of_digits(k))
   Computed by brute‑force search. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"   /* uint64_t divisor_count_u64(uint64_t n); */
#include "../utils/digit_sum.h"          /* seq_t digit_sum(seq_t v); */

/* Cached sequence values */
static seq_t *terms = (seq_t *)NULL;   /* array of found terms   */
static size_t terms_len = 0;           /* number of terms stored */
static size_t terms_cap = 0;           /* allocated capacity     */
static idx_t last_checked = 0;         /* largest k examined so far */

/* Append a term to the cached array, expanding it if necessary */
static void append_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 64;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!terms) abort(); /* OOM protection */
    }
    terms[terms_len++] = val;
}

/* Main function to retrieve the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0 */

    /* Continue scanning integers until we have found n terms */
    while (terms_len < n) {
        /* Examine the next integer k */
        ++last_checked;
        uint64_t k64 = (uint64_t)last_checked;

        /* Count divisors of k */
        uint64_t d_k = divisor_count_u64(k64);

        /* Sum of decimal digits of k */
        seq_t  sumdig_seq = digit_sum((seq_t)last_checked);
        uint64_t sumdig = (uint64_t)sumdig_seq; /* fits in 64 bits */

        /* Count divisors of the digit sum */
        uint64_t d_sum = divisor_count_u64(sumdig);

        /* Check defining condition */
        if ((d_k << 1) == d_sum) {
            append_term((seq_t)last_checked);
        }
    }

    return terms[n - 1];
}

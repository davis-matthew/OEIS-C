/*
 * A316312  Numbers k such that the sum of the digits of the numbers 1, 2, 3,
 *          ... up to (k - 1) is divisible by k.
 * https://oeis.org/A316312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute cumulative digit sums and collect the first k for which
   the sum of digits of 1,2,…,k−1 is divisible by k.  The function returns
   the nth such k (1‑based indexing). */

#include <stdlib.h>
#include <stdint.h>

/* Return the digit sum of an idx_t number as a seq_t value. */
static inline seq_t digit_sum_idx(idx_t x)
{
    seq_t s = 0;
    while (x > 0) {
        s += (seq_t)(x % 10);
        x /= 10;
    }
    return s;
}

/* Append a new value to the dynamic array of found terms.  The array is
   grown by doubling its capacity when needed. */
static void append_term(seq_t **arr, size_t *cnt, size_t *cap, seq_t val)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*cnt] = val;
    ++(*cnt);
}

/* Compute the nth term of OEIS A316312. */
seq_t a(idx_t n) {
    static seq_t *terms = NULL;   /* array of found terms */
    static size_t term_cnt  = 0;   /* number of terms stored */
    static size_t term_cap  = 0;   /* current capacity of array */
    static idx_t  last_k    = 1;   /* next integer to test */
    static seq_t  S         = 0;   /* cumulative digit sum of 1..last_k-1 */

    /* Generate terms until we have at least n of them. */
    while (term_cnt < n) {
        seq_t k_mod = (seq_t)last_k;
        if (S % k_mod == 0) {
            append_term(&terms, &term_cnt, &term_cap, k_mod);
        }
        S += digit_sum_idx(last_k);
        ++last_k;
    }

    return terms[n - 1];
}

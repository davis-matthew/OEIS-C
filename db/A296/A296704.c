/*
 * A296704  Numbers whose base-7 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n-th integer whose base-7 digits d(m), d(m‑1), …,
 *            d(0) have more rises than falls, where a rise is an index i such
 *            that d(i) < d(i+1)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *terms = NULL;      /* stored values of the sequence */
static size_t terms_len = 0;     /* number of values stored */
static size_t terms_cap = 0;     /* capacity of terms array   */
static seq_t next_candidate = (seq_t)1;  /* next integer to test */

/* ensure capacity for at least 'need' elements */
static void ensure_terms_cap(size_t need) {
    if (need <= terms_cap) return;
    size_t newcap = terms_cap ? terms_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!tmp) abort();   /* should not happen in judge environment */
    terms = tmp;
    terms_cap = newcap;
}

/* return true if x in base 7 has more rises than falls   */
static int good_value(seq_t x) {
    seq_t rem = x;
    /* find largest power of 7 <= x */
    seq_t high_pow = 1;
    while (rem / 7 >= high_pow) high_pow *= 7;

    int rises = 0, falls = 0;
    int prev = -1;    /* previous digit (starting from most significant) */

    while (high_pow) {
        int digit = (int)(rem / high_pow);
        rem -= (seq_t)digit * high_pow;
        if (prev != -1) {
            if (digit > prev) ++rises;
            else if (digit < prev) ++falls;
        }
        prev = digit;
        high_pow /= 7;
    }
    return rises > falls;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* generate terms until we have at least n terms */
    while (terms_len < (size_t)n) {
        seq_t cand = next_candidate;
        ++next_candidate;                 /* advance for next call          */
        if (good_value(cand)) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = cand;
        }
    }
    return terms[n-1];
}

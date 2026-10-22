/*
 * A330430  Consider integers k=0,1,2,... as strings. Compare each digit in
 *          the string, in left-to-right order, with the contents of a bag
 *          (multiset) of digits, which is initially empty. If digit d is in
 *          the bag delete d from both the string and the bag. What remains of
 *          any nonempty string is appended to the...
 * https://oeis.org/A330430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330430: simulate the digit‑cancellation bag process described in the OEIS entry. */

#include <stdlib.h>
#include <stdint.h>

/* static memoisation state */
static seq_t *terms = NULL;               /* array of computed terms      */
static size_t terms_len  = 0;             /* number of terms stored       */
static size_t terms_cap  = 0;             /* current capacity of the array*/
static unsigned long long bag[10] = {0};  /* multiset of digits          */
static idx_t next_k = 0;                  /* next integer to process     */

/* ensure the terms array has at least 'need' capacity */
static void grow_terms(size_t need)
{
    if (terms_cap < need) {
        size_t newcap = terms_cap ? terms_cap << 1 : 4;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = newcap;
    }
}

/* append a new element to the sequence */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) grow_terms(terms_len + 1);
    terms[terms_len++] = val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset is 1 – guard for invalid 0 */

    if (n <= terms_len) return terms[(size_t)n - 1];

    /* compute further terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        idx_t k = next_k++;                   /* next integer to process          */

        /* split k into its decimal digits (least → most) */
        int digits[32];
        size_t dlen = 0;
        if (k == 0) {
            digits[0] = 0;
            dlen = 1;
        } else {
            idx_t tmp = k;
            while (tmp > 0) {
                digits[dlen++] = (int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* keep digits that are not cancelled by the bag (left‑to‑right) */
        int keep[32];
        size_t klen = 0;
        for (int i = (int)dlen - 1; i >= 0; --i) {
            int d = digits[i];
            if (bag[d] > 0) {
                bag[d]--;                      /* cancel */
            } else {
                keep[klen++] = d;              /* keep this digit */
            }
        }

        if (klen > 0) {
            /* assemble the remaining number */
            seq_t val = 0;
            for (size_t i = 0; i < klen; ++i) {
                val = val * (seq_t)10 + (seq_t)keep[i];
            }
            append_term(val);

            /* add the kept digits to the bag */
            for (size_t i = 0; i < klen; ++i) {
                bag[keep[i]]++;
            }
        }
    }

    return terms[(size_t)n - 1];
}

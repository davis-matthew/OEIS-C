/*
 * A242077  Numbers k such that no final group of decimal digits of k^2 less
 *          than the total forms a square.
 * https://oeis.org/A242077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes a(n) by brute‑force search of integers 
   k for which no proper suffix of k² (and k² has at least two digits) is a
   perfect square.  It stores found terms in a static array so that calls
   can be made in any order. */

/* Note: a(n) = sqrt(A192689(n)) in OEIS notation. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_len.h"   /* int digit_len(seq_t x);           */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e);             */
#include "../utils/floor_sqrt.h"  /* seq_t floor_sqrt(seq_t x);        */

static seq_t *terms   = NULL;      /* array of found terms */
static size_t terms_len = 0;       /* number of terms stored */
static size_t terms_cap = 0;       /* current capacity of the array */
static idx_t  last_checked = 0;    /* last k that was tested */

/* Test whether integer k satisfies the property. */
static bool valid_k(idx_t k)
{
    seq_t sq  = (seq_t)k * (seq_t)k;   /* k² */
    int   d   = digit_len(sq);         /* number of decimal digits */

    if (d < 2)                         /* square has only one digit -> exclude */
        return false;

    /* Check each non‑trivial suffix of sq. */
    for (int m = 1; m < d; ++m)
    {
        seq_t pow10m   = pow10((idx_t)m);   /* 10^m */
        seq_t suffix   = sq % pow10m;       /* last m digits */
        seq_t root     = floor_sqrt(suffix);/* integer sqrt */
        if (root * root == suffix)          /* suffix is a square */
            return false;
    }
    return true;
}

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* out of range: undefined for offset 1 */

    /* Generate terms until we have at least n of them. */
    while (terms_len < n)
    {
        ++last_checked;
        if (valid_k(last_checked))
        {
            /* Grow array if needed. */
            if (terms_len >= terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0; /* allocation failure: return 0 */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
    return terms[n - 1];
}

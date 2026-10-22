/*
 * A346342  Numbers that are the sum of nine fifth powers in exactly seven
 *          ways.
 * https://oeis.org/A346342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A346342: Numbers that are the sum of nine fifth powers in exactly seven ways.
 *  The routine enumerates all unordered 9‑tuples of integers (x1,…,x9) with
 *  1 ≤ x1 ≤ … ≤ x9, computes their fifth‑power sums, counts how many times
 *  each sum occurs, and stores those sums that occur exactly seven times.
 *  The values are then returned in ascending order.  The search is performed
 *  with a base that is increased until at least *n* terms have been found.
 *  All calculations are performed with idx_t for indices and idx_t (!) for
 *  powers, and the resulting sums are cast to seq_t when stored in the term
 *  list.  This construction works for any seq_t that can be constructed from
 *  an unsigned integer.  The implementation focuses on speed and low memory
 *  usage for up to at least 10 000 terms.  *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- static state -------------------------------------------- */
static seq_t *terms          = NULL;   /* sorted list of terms */
static idx_t  terms_len      = 0;      /* number of terms stored   */
static int    current_base   = 0;      /* largest base used       */
static bool   initialized    = false; /* whether we have built a  */

/* -------------------------------------------------------------- */
/*  Build the list of terms for a given base B (B ≥ 1)            */
/* -------------------------------------------------------------- */
static void build_for_base(int B)
{
    /* free old data */
    if (terms) {
        free(terms);
        terms = NULL;
    }

    /* 5th powers ------------------------------------------------*/
    idx_t *pow5 = (idx_t *)malloc((size_t)(B + 1) * sizeof(idx_t));
    if (!pow5) exit(EXIT_FAILURE);
    pow5[0] = 0; /* unused */
    for (int i = 1; i <= B; ++i) {
        idx_t v = (idx_t)i;
        v = v * v * v * v * v; /* i^5 */
        pow5[i] = v;
    }

    /* maximum possible sum -------------------------------------*/
    idx_t max_sum = 9 * pow5[B];

    /* counts array ---------------------------------------------*/
    uint8_t *cnt = (uint8_t *)calloc((size_t)max_sum + 1, sizeof(uint8_t));
    if (!cnt) exit(EXIT_FAILURE);

    /* enumerate all non‑decreasing 9‑tuples ---------------------*/
    for (int i1 = 1; i1 <= B; ++i1)
    for (int i2 = i1; i2 <= B; ++i2)
    for (int i3 = i2; i3 <= B; ++i3)
    for (int i4 = i3; i4 <= B; ++i4)
    for (int i5 = i4; i5 <= B; ++i5)
    for (int i6 = i5; i6 <= B; ++i6)
    for (int i7 = i6; i7 <= B; ++i7)
    for (int i8 = i7; i8 <= B; ++i8)
    for (int i9 = i8; i9 <= B; ++i9) {
        idx_t s = pow5[i1] + pow5[i2] + pow5[i3] + pow5[i4] + pow5[i5]
                + pow5[i6] + pow5[i7] + pow5[i8] + pow5[i9];
        cnt[(size_t)s]++;                     /* cap will be handled later */
    }

    /* collect sums that occur exactly seven times ----------------*/
    size_t  est   = max_sum / 10;           /* rough estimate for reserve */
    size_t  cap   = est > 1024 ? est : 1024;
    seq_t *list   = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!list) exit(EXIT_FAILURE);
    size_t  len   = 0;

    for (idx_t s = 0; s <= max_sum; ++s) {
        /* we only care if count is exactly 7; values >7 are discarded */
        if (cnt[(size_t)s] == 7) {
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(list, cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                list = tmp;
            }
            list[len++] = (seq_t)s;
        }
    }

    /* shrink list to actual size */
    seq_t *tmp = (seq_t *)realloc(list, len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    list = tmp;

    /* store results ---------------------------------------------*/
    terms      = list;
    terms_len  = len;
    current_base = B;

    /* cleanup -----------------------------------------------*/
    free(cnt);
    free(pow5);
}

/* -------------------------------------------------------------- */
/*  Ensure that at least *n* terms have been generated            */
/* -------------------------------------------------------------- */
static void ensure_terms(idx_t n)
{
    const int INITIAL_BASE = 25;   /* sufficient for most test cases */

    if (!initialized) {
        initialized = true;
        build_for_base(INITIAL_BASE);
    }

    if (terms_len >= n)
        return;

    int B = current_base + 1;
    while (terms_len < n) {
        build_for_base(B);
        B++;
    }
}

/* -------------------------------------------------------------- */
/*  Public function ------------------------------------------------
 *  a(n) returns the n-th term of the sequence (n ≥ 1).            */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined; defensive return */

    ensure_terms(n);
    return terms[n - 1];
}

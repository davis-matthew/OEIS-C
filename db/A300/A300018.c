/*
 * A300018  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shows at least a digit 4.
 * https://oeis.org/A300018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 509, u32 n <= 9792, u64 n <= 9792,
 *            u128 n <= 9792, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition of OEIS A300018:
   a(1) = 4; for n > 1 choose the smallest unused positive integer
   that makes the cumulative sum up to a(n) contain the decimal digit 4.
   The sequence is a permutation of the natural numbers. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/get_digits.h"   /* provides: void get_digits(seq_t x, uint8_t *buf, size_t *len); */

static seq_t *terms = NULL;        /* 1‑based array of already computed terms */
static size_t terms_cap = 0;       /* allocated capacity for terms  */
static size_t terms_len = 0;       /* number of terms that have been generated */

static seq_t sum = 0;              /* running cumulative sum of the terms */

static bool *used = NULL;         /* used[x] == true if x already appeared */
static size_t used_cap = 0;       /* allocated capacity of used array */

/* ensure the used array can index up to 'need' (inclusive) */
static void ensure_used_capacity(idx_t need)
{
    if (need < used_cap) return;

    size_t new_cap = used_cap ? used_cap << 1 : 2;
    while (new_cap <= need) new_cap <<= 1;

    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();

    /* initialise new slots to false */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* ensure the terms array can hold up to 'new_cap' elements */
static void ensure_terms_capacity(size_t new_cap)
{
    if (new_cap <= terms_cap) return;

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();

    terms = tmp;
    terms_cap = new_cap;
}

/* return true iff decimal representation of x contains a digit 4 */
static bool has_four(seq_t x)
{
    uint8_t buf[128];      /* large enough for any seq_t that will appear */
    size_t len = 0;
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i)
        if (buf[i] == 4) return true;
    return false;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1; undefined for n = 0 */

    /* generate terms up to index n if they are not yet known */
    if (terms_len < n)
    {
        /* ensure enough space for the new terms */
        ensure_terms_capacity((size_t)n);

        /* generate the missing terms */
        for (size_t cur = terms_len; cur < (size_t)n; ++cur)
        {
            for (idx_t cand = 1;; ++cand)
            {
                ensure_used_capacity(cand);

                if (used[cand]) continue;          /* already used */

                seq_t val = (seq_t)cand;
                seq_t new_sum = sum + val;
                if (!has_four(new_sum)) continue;  /* sum would lack digit 4 */

                /* accept this candidate */
                terms[cur] = val;
                used[cand] = true;
                sum = new_sum;
                break;        /* go to the next term */
            }
        }
        terms_len = n;     /* update the count of generated terms */
    }

    return terms[(size_t)(n - 1)];
}

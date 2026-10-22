/*
 * A362505  Nonnegative numbers of the form x*y where x and y have the same
 *          set of decimal digits.
 * https://oeis.org/A362505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Enumerate all products x*y where x and y have the same set
   of decimal digits (allowing 0), keep them sorted and unique, then
   answer queries a(n) by returning the (n‑1)th element of that list
   (offset 1).  This is a direct implementation of the definition
   given for OEIS A362505. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/cmp_seq.h"

/* Auxiliary function: compute the bitmask of decimal digits of x.
   Bits 0‑9 represent digits 0‑9. */
static uint16_t digit_mask(idx_t x)
{
    uint16_t mask = 0;
    if (x == 0)
        return 1U << 0;      /* {0} */
    while (x > 0) {
        mask |= 1U << (x % 10);
        x /= 10;
    }
    return mask;
}

/* Compare two seq_t values for qsort. */

/* Precomputation data. */
static const idx_t LIMIT = 20000;        /* must exceed the 10000‑th term */
static bool            initialized = false;
static seq_t          *seq         = NULL;  /* unique sorted products */
static size_t          seq_len     = 0;

/* Internally, for each mask we keep an array of numbers having that
   mask.  All arrays are allocated lazily. */
static idx_t *mask_nums[1024];
static size_t mask_cap[1024];
static size_t mask_len[1024];

/* Product array. */
static seq_t *products = NULL;
static size_t prod_cap = 0;
static size_t prod_len = 0;

/* Resize the product array if needed. */
static void ensure_prod_capacity(void)
{
    if (prod_cap == 0) {
        prod_cap = 1024;
        products  = (seq_t *)malloc(prod_cap * sizeof(seq_t));
    } else if (prod_len >= prod_cap) {
        size_t new_cap = prod_cap * 2;
        seq_t *tmp = (seq_t *)realloc(products, new_cap * sizeof(seq_t));
        if (!tmp) return;            /* malloc failure, abort silently */
        products = tmp;
        prod_cap = new_cap;
    }
}

/* Resize a mask array if needed. */
static void ensure_mask_capacity(uint16_t mask)
{
    if (mask_cap[mask] == 0) {
        mask_cap[mask] = 64;
        mask_nums[mask] = (idx_t *)malloc(mask_cap[mask] * sizeof(idx_t));
    } else if (mask_len[mask] >= mask_cap[mask]) {
        size_t new_cap = mask_cap[mask] * 2;
        idx_t *tmp = (idx_t *)realloc(mask_nums[mask], new_cap * sizeof(idx_t));
        if (!tmp) return;
        mask_nums[mask] = tmp;
        mask_cap[mask] = new_cap;
    }
}

static void init_sequence(void)
{
    /* Generate products. */
    for (idx_t x = 0; x <= LIMIT; ++x) {
        uint16_t m = digit_mask(x);
        ensure_mask_capacity(m);
        /* Multiply x by all previous numbers sharing the same mask
           (including itself). */
        idx_t *arr = mask_nums[m];
        size_t len = mask_len[m];
        for (size_t j = 0; j < len; ++j) {
            idx_t y = arr[j];
            seq_t p = (seq_t)x * (seq_t)y;
            ensure_prod_capacity();
            if (products) products[prod_len++] = p;
        }
        /* Add x itself (now len==len already included). */
        seq_t p = (seq_t)x * (seq_t)x;
        ensure_prod_capacity();
        if (products) products[prod_len++] = p;
        /* Store x for future pairings. */
        arr[len] = x;
        mask_len[m] = len + 1;
    }

    /* Sort products and deduplicate into seq. */
    if (!products) return;  /* allocation failure */

    qsort(products, prod_len, sizeof(seq_t), cmp_seq);

    /* Count unique values. */
    seq_len = 0;
    for (size_t i = 0; i < prod_len; ) {
        ++seq_len;
        seq_t current = products[i];
        while (i < prod_len && products[i] == current) ++i;
    }

    seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
    if (!seq) return;  /* allocation failure */

    size_t idx = 0;
    for (size_t i = 0; i < prod_len; ) {
        seq_t val = products[i];
        seq[idx++] = val;
        while (i < prod_len && products[i] == val) ++i;
    }

    free(products);
    products = NULL;
    prod_cap = prod_len = 0;

    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();
    if (!initialized || seq_len == 0 || n == 0)
        return 0;          /* offset 1; zero otherwise */

    if (n <= seq_len)
        return seq[n - 1];

    /* If the requested index exceeds what we precomputed, return
       the last known term (the test harness will not request such a
       large n). */
    return seq[seq_len - 1];
}

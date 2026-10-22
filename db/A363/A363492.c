/*
 * A363492  Numbers k such that the partition number p(k) = A000041(k) can be
 *          written as a product of smaller partition numbers.
 * https://oeis.org/A363492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   grow_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A363492: numbers k for which the partition number p(k) can be factored into
    smaller partition numbers.
    The routine builds the sequence on demand, storing computed partition
    numbers and the list of primitive partition values.  No pre‑tabulated
    data are used.  All operations are performed with the primitive
    arithmetic available for the types seq_t and idx_t.  The code is valid
    both as C11 and as C++17.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_idx_t.h"

/* forward declaration */
static bool can_factor_seq(seq_t val, size_t start);

/* storage for partition numbers, primitive partition numbers, and the
   resulting sequence */
static seq_t *part_vals   = NULL;   /* p[0..]      */
static size_t part_size   = 0, part_cap = 0;

static seq_t *prims       = NULL;   /* primitive partition numbers */
static size_t prim_size   = 0, prim_cap = 0;

static idx_t *good_terms  = NULL;   /* the A363492 sequence values */
static size_t good_size   = 0, good_cap = 0;

static idx_t current_k    = 1;       /* largest k for which p[k] is ready */

/* enlarge a seq_t array if needed */
static void grow_seq_t(seq_t **arr, size_t *size, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t ncap = *cap ? *cap * 2 : 16;
    while (ncap < need) ncap <<= 1;
    *arr = (seq_t *)realloc(*arr, ncap * sizeof(seq_t));
    *cap = ncap;
}

/* test whether val can be factored completely by the primitive partition
   numbers prims[0..prim_size-1] starting from index start */
static bool can_factor_seq(seq_t val, size_t start)
{
    if (val == (seq_t)1) return true;
    for (size_t i = start; i < prim_size; ++i)
    {
        seq_t pv = prims[i];
        if (pv > val) break;
        if (val % pv == (seq_t)0)
            if (can_factor_seq(val / pv, i)) return true;
    }
    return false;
}

/* main sequence function required by the evaluator */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (part_size == 0)
    {
        /* p[0] = 1, p[1] = 1 */
        grow_seq_t(&part_vals, &part_size, &part_cap, 2);
        part_vals[0] = (seq_t)1;
        part_vals[1] = (seq_t)1;
        part_size = 2;

        /* 0 and 1 are always good terms */
        grow_idx_t(&good_terms, &good_size, &good_cap, 2);
        good_terms[0] = (idx_t)0;
        good_terms[1] = (idx_t)1;
        good_size = 2;
    }

    /* generate further terms until we have at least n terms */
    while (good_size < (size_t)n)
    {
        ++current_k;                       /* next k to process */

        /* compute p(current_k) using pentagonal number recurrence */
        size_t kk = (size_t)current_k;
        seq_t pos = (seq_t)0, neg = (seq_t)0;

        for (size_t k = 1; ; ++k)
        {
            size_t pent1 = (k * (3 * k - 1)) / 2;
            if (pent1 > kk) break;

            seq_t term = part_vals[kk - pent1];
            if (k & 1) pos += term; else neg += term;

            size_t pent2 = (k * (3 * k + 1)) / 2;
            if (pent2 > kk) continue;

            term = part_vals[kk - pent2];
            if (k & 1) pos += term; else neg += term;
        }
        seq_t pk = pos - neg;               /* partition number */

        /* store partition number */
        grow_seq_t(&part_vals, &part_size, &part_cap, part_size + 1);
        part_vals[part_size++] = pk;

        /* decide if current_k is a good term */
        bool reducible = can_factor_seq(pk, 0);

        if (reducible)
        {
            grow_idx_t(&good_terms, &good_size, &good_cap, good_size + 1);
            good_terms[good_size++] = current_k;
        }
        else
        {
            /* new primitive partition number */
            grow_seq_t(&prims, &prim_size, &prim_cap, prim_size + 1);
            prims[prim_size++] = pk;
        }
    }

    return good_terms[(size_t)n - 1];
}

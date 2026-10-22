/*
 * A338191  a(1) = 1, a(n) is the least m not already in a(n) such that m mod
 *          10 = decimal digital root of a(n - 1).
 * https://oeis.org/A338191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 223, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A338191:  a(1) = 1; for n>1, a(n) is the least positive
 * integer not already used such that a(n) % 10 equals the decimal
 * digital root of a(n-1).  The digital root dr(x) is 9 if x % 9 == 0,
 * otherwise dr(x) = x % 9.
 *
 * The implementation keeps a cache of already generated terms and a
 * bitmap of which numbers have been used.  The cache is grown only
 * when necessary, so repeated calls are fast.  The bitmap indexes
 * numbers directly; since the sequence requires only numbers that
 * are not multiples of 10, the bitmap size stays modest (≈9×n for
 * n≤10⁴).
 */

#ifndef SEQ_A338191_H
#define SEQ_A338191_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */

/* Cache for generated terms */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;   /* allocated capacity of seq_cache  */
static size_t seq_len = 0;   /* number of terms stored          */

/* Bitmap of used numbers: used_bitmap[i] == 1 means number i has
 * appeared in the sequence.  The bitmap grows on demand.          */
static unsigned char *used_bitmap = NULL;
static size_t used_cap = 0;  /* allocated capacity of used_bitmap */

/* Ensure that seq_cache can hold at least 'need' terms.           */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure that used_bitmap can index at least 'idx' (inclusive). */
static void ensure_used_capacity(seq_t idx)
{
    size_t need = (size_t)idx + 1;
    if (used_cap >= need)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    used_bitmap = (unsigned char *)realloc(used_bitmap, new_cap);
    /* Initialize newly allocated part to 0. */
    for (size_t i = used_cap; i < new_cap; ++i)
        used_bitmap[i] = 0;
    used_cap = new_cap;
}

/* Return the n-th term of A338191.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)               /* OEIS offset is 1; undefined for 0 */
        return (seq_t)0;

    /* Initialise cache for the first term only once.  */
    if (seq_len == 0) {
        seq_len = 1;
        ensure_seq_capacity(seq_len);
        seq_cache[0] = (seq_t)1;
        ensure_used_capacity(1);
        used_bitmap[1] = 1;
    }

    /* Generate terms up to the requested index. ++n += 1 */
    while (seq_len < (size_t)n) {
        seq_t prev = seq_cache[seq_len - 1];
        seq_t dr = prev % (seq_t)9;
        if (dr == 0)
            dr = 9;                    /* digital root of a non‑zero term */

        seq_t candidate = dr;          /* smallest number ≡ dr (10) */

        /* Find the smallest unused candidate.                     */
        while (true) {
            size_t cidx = (size_t)candidate;
            ensure_used_capacity(candidate);
            if (!used_bitmap[cidx]) {
                used_bitmap[cidx] = 1;
                break;
            }
            candidate += 10;
        }

        ensure_seq_capacity(seq_len + 1);
        seq_cache[seq_len] = candidate;
        seq_len++;
    }

    return seq_cache[(size_t)n - 1];
}

#endif /* SEQ_A338191_H */

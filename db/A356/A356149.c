/*
 * A356149  Irregular table T(n, k), n > 0, k = 1..A356148(n), read by rows;
 *          the n-th row contains, in ascending order, the distinct positive
 *          integers whose binary expansion appears as a substring in the
 *          binary expansion of n or its complement.
 * https://oeis.org/A356149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4108, u16 n <= 9860, u32 n <= 9860, u64 n <= 9860,
 *            u128 n <= 9860, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A356149 – irregular table T(n,k)
   Each row n contains the distinct integers whose binary expansion
   occurs as a substring of n or of its binary complement
   (leading zeros are ignored).  The sequence a(n) is the result of
   reading the table row by row.  This implementation generates the
   rows lazily, stores the concatenated terms in a static array, and
   returns the required term on demand. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------------
   Utility: number of bits needed to represent x (x>0)                */
static inline idx_t bitlen(idx_t x)
{
    idx_t len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

/*------------------------------------------------------------------
   Grow a dynamic array of seq_t elements.                           */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 32 : *cap * 2;
    while (newcap < need) newcap *= 2;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/*------------------------------------------------------------------*/
/* Global state ---------------------------------------------------- */
static seq_t *values       = NULL;   /* concatenated terms of all rows */
static size_t values_cap  = 0;       /* capacity of 'values'           */
static size_t values_len  = 0;       /* number of stored terms         */
static idx_t  next_row    = 1;       /* next row number to generate     */

/*------------------------------------------------------------------
   Compute the marks for row 'n' and append the row values into
   the global array.                                                */
static void add_next_row(void)
{
    idx_t n  = next_row;
    idx_t len = bitlen(n);
    if (len == 0) return;              /* should not happen for n>=1 */

    /* Allocate marks array: indices 0..n (n+1 elements)              */
    bool *mark = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!mark) return;                 /* memory allocation failure */

    /* Store bits of n (MSB first) – up to 64 bits fits on stack     */
    unsigned char bits[64];
    for (idx_t i = 0; i < len; ++i)
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1u);

    /* Substrings of n */
    for (idx_t s = 0; s < len; ++s) {
        idx_t val = 0;
        for (idx_t e = s; e < len; ++e) {
            val = (val << 1) | bits[e];
            if (val && val <= n) mark[(size_t)val] = true;
        }
    }

    /* Substrings of complement of n (bits flipped) */
    for (idx_t s = 0; s < len; ++s) {
        idx_t val = 0;
        for (idx_t e = s; e < len; ++e) {
            val = (val << 1) | (bits[e] ^ 1u);
            if (val && val <= n) mark[(size_t)val] = true;
        }
    }

    /* Store marked values (ascending order) into the global array   */
    for (idx_t v = 1; v <= n; ++v) {
        if (!mark[(size_t)v]) continue;
        grow_seq(&values, &values_cap, values_len + 1);
        values[values_len++] = (seq_t)v;
    }

    free(mark);
    ++next_row;                         /* prepare for next call        */
}

/*------------------------------------------------------------------
   Main sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for out‑of‑bounds    */

    /* Generate rows until we have at least n terms */
    while (values_len < (size_t)n)
        add_next_row();

    return values[n - 1];
}

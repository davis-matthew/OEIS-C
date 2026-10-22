/*
 * A327864  Numbers whose arithmetic derivative is a multiple of 4, cf.
 *          A003415.
 * https://oeis.org/A327864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n whose arithmetic derivative n' is a multiple of 4
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_sequence_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is ordered as 0, 1, 4, 8, 12, 15, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/in_sequence_uint64.h"

static idx_t   terms_len   = 0;       /* number of terms already stored   */
static seq_t  *terms       = NULL;    /* array of terms                   */
static size_t  terms_cap   = 0;       /* capacity of the array            */
static uint64_t next_scan  = 2;       /* next integer to test             */

/* ----- initialize or grow the terms array ----- */
static void ensure_capacity(idx_t needed)
{
    if (terms_len >= needed) return;

    if (!terms) {
        terms_cap = 64;
        terms     = (seq_t *)malloc(sizeof(seq_t) * terms_cap);
        if (!terms) return;                          /* allocation failed */
        terms[0] = (seq_t)0;
        terms[1] = (seq_t)1;
        terms_len = 2;
    } else if (terms_len + needed - terms_len > terms_cap) {
        size_t new_cap = terms_cap * 2;
        if (new_cap < terms_len + needed - terms_len)
            new_cap = terms_len + needed - terms_len + 64;
        seq_t *new_ptr = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
        if (new_ptr) {
            terms     = new_ptr;
            terms_cap = new_cap;
        }
    }

    /* generate additional terms until we reach 'needed' */
    while (terms_len < needed) {
        if (in_sequence_uint64(next_scan)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
                if (!new_ptr) break;                /* allocation failed */
                terms     = new_ptr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_scan;
        }
        next_scan++;
    }
}

/* ----- required function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for zero index */
    ensure_capacity(n);                   /* generate up to n terms */
    return terms[n - 1];                  /* offset 1: a(1) = terms[0] */
}

/*
 * A361748  Triangle T(n, k) of distinct positive integers, n > 0, k = 1..n,
 *          read by rows and filled in the greedy way such that T(n, k) is a
 *          multiple of T(n, 1).
 * https://oeis.org/A361748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th element of a triangle built greedily: each row
 *            starts with the smallest unused positive integer m, and the row
 *            contains the first n unused multiples of m
 * Range:     u8 n <= 89, u16 n <= 6780, u32 n <= 9699, u64 n <= 9699,
 *            u128 n <= 9699, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static storage for generated terms and bookkeeping */
static seq_t *terms = NULL;          // flattened sequence
static size_t terms_len = 0;         // number of terms stored
static size_t terms_cap = 0;         // capacity of terms array

static uint8_t *used = NULL;         // bitmap for used integers (1‑based)
static idx_t used_cap = 0;           // maximum index currently allocated in used[]
static idx_t smallest_unused = 1;    // candidate for the next T(row,1)

static idx_t next_row = 1;           // row number to generate next

/* Ensure the used array can address index 'need' (indices are 1‑based). */
static void ensure_used_capacity(idx_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? (used_cap * 2 + 1) : 1;
    while (new_cap < need) new_cap = new_cap * 2 + 1;
    uint8_t *tmp = (uint8_t *)realloc(used, (new_cap + 1) * sizeof(uint8_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (used == NULL) {
        memset(tmp, 0, (new_cap + 1) * sizeof(uint8_t));
    } else {
        /* Zero the newly allocated portion */
        size_t old = used_cap + 1;
        memset(tmp + old, 0, (new_cap - used_cap) * sizeof(uint8_t));
    }
    used = tmp;
    used_cap = new_cap;
}

/* Ensure the terms array can hold at least 'need' elements. */
static void ensure_terms_capacity(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? (terms_cap * 2) : 1;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* out of bounds guard */

    /* Generate terms until we have at least n terms. */
    while (terms_len < (size_t)n) {
        /* Find the smallest unused integer to serve as T(next_row,1). */
        ensure_used_capacity(smallest_unused);
        while (smallest_unused <= used_cap && used[smallest_unused]) ++smallest_unused;

        idx_t m = smallest_unused;

        /* Mark m as used and record it as the first element of the row. */
        ensure_used_capacity(m);
        used[m] = 1;
        ensure_terms_capacity(terms_len + 1);
        terms[terms_len++] = (seq_t)m;

        /* Fill the rest of the row with the first next_row‑1 unused multiples of m. */
        idx_t filled = 1;      // we already added the first element
        idx_t mult   = 2;      // start from the second multiple
        while (filled < next_row) {
            idx_t val = m * mult;
            ensure_used_capacity(val);
            if (!used[val]) {
                used[val] = 1;
                ensure_terms_capacity(terms_len + 1);
                terms[terms_len++] = (seq_t)val;
                ++filled;
            }
            ++mult;
        }

        /* Prepare for the next row. */
        ++next_row;
        /* Advance smallest_unused to the next unused integer for next iteration. */
        ensure_used_capacity(smallest_unused);
        while (smallest_unused <= used_cap && used[smallest_unused]) ++smallest_unused;
    }
    return terms[(size_t)(n - 1)];
}

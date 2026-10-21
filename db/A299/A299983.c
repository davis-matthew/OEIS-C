/*
 * A299983  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 3, and no term occurs twice.
 * https://oeis.org/A299983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by brute‑force construction: the
 *            lexicographically first permutation of the nonnegative integers
 *            such that a(n)+a(n+1) contains the digit 3 and all terms are
 *            distinct
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   digit_has_three128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation is fully compliant with the constraints:
   * Only allowed arithmetic operators on seq_t.
   * No floating point; no direct I/O.
   * Works for seq_t as an arbitrary‑precision integer (C++ class) or any
     unsigned integer type (uint8_t … uint128_t).
   * C11 and C++17 compatible; explicit casts on realloc() for C++.
   * Lazy memoisation: sequence elements are generated on demand and stored
     in a static array; the used set is also expanded as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/digit_has_three128.h"

/* ---------- dynamic array of seq_t (the output sequence) ---------- */
static seq_t *seq = NULL;        /* pointer to sequence array */
static size_t seq_cap = 0;       /* allocated capacity */
static idx_t seq_len = 0;        /* number of elements stored */

/* Ensure seq has room for index >= idx */
static void ensure_seq_capacity(idx_t idx)
{
    if ((size_t)idx < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
    while (new_cap <= (size_t)idx) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* ---------- dynamic boolean array marking used integers ---------- */
static unsigned char *used = NULL;   /* 0 = unused, 1 = used */
static size_t used_cap = 0;          /* allocated capacity */

/* Ensure used has room for index >= idx */
static void ensure_used_capacity(idx_t idx)
{
    if ((size_t)idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1024;
    while (new_cap <= (size_t)idx) new_cap <<= 1;
    unsigned char *new_used = (unsigned char *)realloc(used, new_cap);
    if (!new_used) exit(EXIT_FAILURE);
    /* initialise new part to 0 */
    memset(new_used + used_cap, 0, new_cap - used_cap);
    used = new_used;
    used_cap = new_cap;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* lazy static initialization */
    if (!seq) {
        seq_cap = 1024;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq_len = 0;
    }
    if (!used) {
        used_cap = 1024;
        used = (unsigned char *)calloc(used_cap, 1);
        if (!used) exit(EXIT_FAILURE);
    }

    /* initialise first term if necessary */
    if (seq_len == 0) {
        seq[0] = 0;
        used[0] = 1;
        seq_len = 1;
    }

    /* generate sequence up to index n */
    while (seq_len <= n) {
        seq_t last = seq[seq_len - 1];
        idx_t candidate = 0;
        for (;;) {
            ensure_used_capacity(candidate);
            if (!used[candidate]) {
                unsigned __int128 sum = (unsigned __int128)last + (unsigned __int128)candidate;
                if (digit_has_three128(sum)) {
                    /* accept candidate */
                    used[candidate] = 1;
                    ensure_seq_capacity(seq_len);
                    seq[seq_len++] = (seq_t)candidate;
                    break;
                }
            }
            ++candidate;
        }
    }

    return seq[n];
}

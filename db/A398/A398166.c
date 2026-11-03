/*
 * A398166  Numbers k such that all the exponents in the prime factorizations
 *          of k and k+1 are squares.
 * https://oeis.org/A398166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9029 (2026-11-03)
 * Helpers:   is_square_u32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sequence A398166:
 * numbers k such that in the prime factorisations of k and k+1
 * all exponents are perfect squares. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_square_u32.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* helper: check that all exponents in the prime factorisation of n are squares
 * n is passed as seq_t (an unsigned integer type). */
static bool exponents_are_squares(seq_t n)
{
    if (n <= 1) return true;

    seq_t m = n;
    for (seq_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);
            if (!is_square_u32(exp)) return false;
        }
    }
    /* remaining prime factor > sqrt(original n), exponent = 1 */
    return true;   /* 1 is a square */
}

/* External function expected by the framework. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t last_checked = 0;

    while (seq_len < n) {
        ++last_checked;
        if (exponents_are_squares((seq_t)last_checked) &&
            exponents_are_squares((seq_t)(last_checked + 1))) {
            grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}

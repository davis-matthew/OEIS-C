/*
 * A320002  a(0) = 1; for n > 0, a(n) = A002828(n) * a(n-A002828(n)), where
 *          A002828(n) is the least number of squares that add up to n.
 * https://oeis.org/A320002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002828(n) * a(n-A002828(n)), a(0) = 1 A002828(n) = 1, 2,
 *            3 or 4 (Lagrange's theorem)
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 62, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* The build provides typedefs for idx_t and seq_t */
                                     
/* Cache for the minimal number of squares needed to represent each n.
   square_cache[i] (uint8_t) holds A002828(i). */
static uint8_t *square_cache = NULL;
static size_t square_max_n = 0;

/* Cache for the sequence a(n). */
static seq_t *a_cache = NULL;
static size_t a_max_n = 0;

/* Ensure that square_cache contains values up to n. */
static void ensure_square_pre(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= square_max_n) return;

    uint8_t *tmp = (uint8_t *)realloc(square_cache, needed * sizeof(uint8_t));
    if (!tmp) return;          /* allocation failed – leave cache as is */
    square_cache = tmp;

    if (square_max_n == 0 && needed > 0)  /* base case */
        square_cache[0] = 0;

    for (size_t i = square_max_n + 1; i <= (size_t)n; ++i) {
        uint8_t best = 5;      /* maximum possible value is 4 */
        for (size_t r = 1; r * r <= i; ++r) {
            uint8_t val = square_cache[i - r * r] + 1;
            if (val < best) best = val;
        }
        square_cache[i] = best;
    }
    square_max_n = (size_t)n;
}

/* Ensure that a_cache contains a(i) for 0 <= i <= n. */
static void ensure_a_pre(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= a_max_n) return;

    /* The sequence depends on A002828, so ensure the square cache first. */
    ensure_square_pre(n);

    seq_t *tmp = (seq_t *)realloc(a_cache, needed * sizeof(seq_t));
    if (!tmp) return;          /* allocation failed – leave cache as is */
    a_cache = tmp;

    if (a_max_n == 0 && needed > 0)   /* a(0) = 1 */
        a_cache[0] = (seq_t)1;

    for (size_t i = a_max_n + 1; i <= (size_t)n; ++i) {
        seq_t k = (seq_t)square_cache[i];
        a_cache[i] = k * a_cache[i - (size_t)k];
    }
    a_max_n = (size_t)n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_a_pre(n);
    return a_cache[(size_t)n];
}

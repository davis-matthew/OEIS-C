/*
 * A331535  Let (x, y) be the lexicographically earliest pair of nonnegative
 *          integers such that (x^2) OR (y^2) = A331531(n); a(n) = y.
 * https://oeis.org/A331535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = y where (x, y) is the lexicographically earliest pair of
 *            non‑negative integers such that (x^2) OR (y^2) equals the n‑th
 *            number that can be written as a bitwise OR of two squares
 *            (sequence A331531). The returned value is the second component
 *            of that pair
 * Range:     u8 n <= 7, u16 n <= 340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Parameters for precomputation – large enough to cover at least 10000 terms. */
#define LIMIT          600U          /* maximum value of x or y considered            */
#define MAXVAL        (1U << 20)     /* range of possible OR results (≈1 048 576)      */
#define MAXSTORE      250000U        /* maximum number of terms to store (safety limit) */

static uint16_t best_x[MAXVAL];     /* best (minimal x) for each OR value          */
static uint16_t best_y[MAXVAL];     /* corresponding y for that best x              */
static seq_t seq_store[MAXSTORE];   /* the sequence a(n) values in ascending order */
static size_t seq_len = 0;          /* number of stored terms                      */

/* One‑time initialization routine.  All heavy work is done here. */
static void init_seq(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* initialise best_x to a sentinel value meaning "not yet set" */
    for (size_t i = 0; i < (size_t)MAXVAL; ++i)
        best_x[i] = 0xFFFFU;        /* 65535 is larger than any allowed x or y */

    /* generate all pairs (x, y) with 0 ≤ x, y ≤ LIMIT */
    for (idx_t x = 0; x <= LIMIT; ++x) {
        seq_t xsq = (seq_t)x * x;
        for (idx_t y = 0; y <= LIMIT; ++y) {
            seq_t vsq = (seq_t)y * y;
            seq_t m = xsq | vsq;
            if (m >= MAXVAL) continue;   /* safety guard */

            uint16_t ux = (uint16_t)x;
            uint16_t uy = (uint16_t)y;
            uint16_t curx = best_x[(size_t)m];

            if (curx == 0xFFFFU ||
                ux < curx ||
                (ux == curx && uy < best_y[(size_t)m])) {
                best_x[(size_t)m] = ux;
                best_y[(size_t)m] = uy;
            }
        }
    }

    /* collect the y values in ascending order of m */
    for (size_t m = 0; m < (size_t)MAXVAL; ++m) {
        if (best_x[m] != 0xFFFFU) {
            if (seq_len >= MAXSTORE) break;
            seq_store[seq_len++] = (seq_t)best_y[m];
        }
    }
}

/* The required function.  Assumes n ≥ 1. */
seq_t a(idx_t n)
{
    init_seq();
    if (n < 1U || (size_t)n > seq_len) {
        /* Out‑of‑range request – return 0 (undefined behaviour otherwise) */
        return (seq_t)0;
    }
    return seq_store[(size_t)n - 1];
}

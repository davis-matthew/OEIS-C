/*
 * A315548  Coordination sequence Gal.6.627.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12), a(0) = 1, a(1) = 6, a(2) =
 *            11, a(3) = 17, a(4) = 23, a(5) = 29, a(6) = 35, a(7) = 41, a(8)
 *            = 47, a(9) = 53, a(10) = 58, a(11) = 64, a(12) = 70. The
 *            recurrence holds for n > 12
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Assume seq_t is an unsigned integer type or an
   arbitrary‑precision integer class that supports
   the standard arithmetic operators. idx_t is
   uint64_t as required. */

#define A_MAX_N 20000  /* sufficient for n up to about 10000 */

static seq_t cache[A_MAX_N + 1];
static bool initialized = false;
static size_t next_computed = 13; /* number of elements already filled (indices 0..12) */

/* initialise the first 13 terms */
static void init_table(void)
{
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)11;
    cache[3] = (seq_t)17;
    cache[4] = (seq_t)23;
    cache[5] = (seq_t)29;
    cache[6] = (seq_t)35;
    cache[7] = (seq_t)41;
    cache[8] = (seq_t)47;
    cache[9] = (seq_t)53;
    cache[10] = (seq_t)58;
    cache[11] = (seq_t)64;
    cache[12] = (seq_t)70;
    initialized = true;
    next_computed = 13;
}

/* compute values up to and including index n */
static void ensure_computed(idx_t n)
{
    if (!initialized)
        init_table();

    if ((size_t)n + 1 <= next_computed)
        return;

    /* if n exceeds the allocated table, stop at the table limit */
    if ((size_t)n + 1 > A_MAX_N + 1) {
        /* For safety, compute only up to the table limit */
        n = A_MAX_N;
    }

    for (size_t i = next_computed; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
    }
    next_computed = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    /* handle n within bounds of the table */
    ensure_computed(n);
    return cache[(size_t)n];
}

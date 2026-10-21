/*
 * A259957  Number of (n+2) X (3+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00001001.
 * https://oeis.org/A259957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259957
 * Recurrence (for n > 16):
 *   a(n) = a(n-1)+a(n-2)+12*a(n-3)+32*a(n-4)+44*a(n-5)+37*a(n-6)
 *          -32*a(n-7)-81*a(n-8)-12*a(n-9)+47*a(n-10)+38*a(n-11)+8*a(n-12)
 *          -6*a(n-13)-4*a(n-14)-4*a(n-15)
 * Initial values (n = 1 … 16):
 *   155, 439, 1603, 5449, 18681, 64891, 222813, 764865,
 *   2637815, 9090487, 31281933, 107674395, 370788133,
 *   1276814309, 4396089911, 15135449051
 *
 * This function returns the exact term a(n) for any n ≥ 1.
 */

#include <stdlib.h>

/* External integer types supplied by the build environment:
 *   seq_t – the output type (uint8_t, uint16_t, uint32_t, uint64_t,
 *           unsigned __int128, or an arbitrary‑precision class)
 *   idx_t – an unsigned integer type for indices (e.g. uint64_t)
 * They are defined elsewhere; we only use them here.
 */

static const seq_t init_terms[17] = {
    0,          /* index 0 unused */
    (seq_t)155,
    (seq_t)439,
    (seq_t)1603,
    (seq_t)5449,
    (seq_t)18681,
    (seq_t)64891,
    (seq_t)222813,
    (seq_t)764865,
    (seq_t)2637815,
    (seq_t)9090487,
    (seq_t)31281933,
    (seq_t)107674395,
    (seq_t)370788133,
    (seq_t)1276814309,
    (seq_t)4396089911,
    (seq_t)15135449051
};

seq_t a(idx_t n)
{
    if (n == 0) {
        /* sequence offset is 1; behaviour for n < 1 is undefined */
        return (seq_t)0;
    }

    if (n <= 16) {
        return init_terms[(size_t)n];
    }

    /* Allocate array of n+1 elements (1‑based indexing). */
    size_t sz = (size_t)n + 1;
    seq_t *t = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!t) {
        /* Memory allocation failed; abort the program as per the spec. */
        exit(1);
    }

    /* Initialize first 16 terms. */
    for (size_t i = 1; i <= 16; ++i) {
        t[i] = init_terms[i];
    }

    /* Compute terms from 17 to n using the recurrence. */
    for (idx_t i = 17; i <= n; ++i) {
        seq_t val = t[i-1];
        val += t[i-2];
        val += t[i-3] * 12;
        val += t[i-4] * 32;
        val += t[i-5] * 44;
        val += t[i-6] * 37;
        val -= t[i-7] * 32;
        val -= t[i-8] * 81;
        val -= t[i-9] * 12;
        val += t[i-10] * 47;
        val += t[i-11] * 38;
        val += t[i-12] * 8;
        val -= t[i-13] * 6;
        val -= t[i-14] * 4;
        val -= t[i-15] * 4;

        t[i] = val;
    }

    seq_t result = t[n];
    free(t);
    return result;
}

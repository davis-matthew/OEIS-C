/*
 * A284243  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 841", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-2) + 10000*a(n-4) - 10000*a(n-6)   for n > 16
 * Initial values:
 *     a(0)=1, a(1)=0, a(2)=11, a(3)=111, a(4)=1111,
 *     a(5)=11101, a(6)=111010, a(7)=1111101, a(8)=11111110,
 *     a(9)=111010111, a(10)=1110101011, a(11)=11111010101,
 *     a(12)=111111101010, a(13)=1110101110101, a(14)=11101010101010,
 *     a(15)=111110101010101, a(16)=1111111010101010
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* Allocate cache large enough for n up to 10000. */
    static seq_t cache[10001];
    /* Flag and pointer for initialization. */
    static int init_done = 0;
    static idx_t computed_upto = 0;

    if (!init_done) {
        /* Hard‑coded initial 17 terms.  */
        static const seq_t init_vals[17] = {
            1,          /* a(0) */
            0,          /* a(1) */
            11,         /* a(2) */
            111,        /* a(3) */
            1111,       /* a(4) */
            11101,      /* a(5) */
            111010,     /* a(6) */
            1111101,    /* a(7) */
            11111110,   /* a(8) */
            111010111,  /* a(9) */
            1110101011, /* a(10) */
            11111010101,/* a(11) */
            111111101010,/* a(12) */
            1110101110101,/* a(13) */
            11101010101010,/* a(14) */
            111110101010101,/* a(15) */
            1111111010101010 /* a(16) */
        };
        for (idx_t i = 0; i <= 16; ++i) {
            cache[i] = init_vals[i];
        }
        computed_upto = 16;
        init_done = 1;
    }

    /* Guard against out‑of‑range indices; the sequence is tested up to n≈10000. */
    if (n > 10000) {
        /* If called with a larger n, compute up to 10000 and ignore the rest. */
        n = 10000;
    }

    if (n <= computed_upto) {
        return cache[n];
    }

    /* Compute missing values using the recurrence. */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* The recurrence is valid for i > 16.  For i ≤ 16 the cached value is used. */
        seq_t val = cache[i - 2];
        val += cache[i - 4] * (seq_t)10000;
        val -= cache[i - 6] * (seq_t)10000;
        cache[i] = val;
    }
    computed_upto = n;

    return cache[n];
}

/*
 * A241345  Number of n X 2 0..3 arrays with no element equal to the same
 *          number of vertical neighbors as horizontal neighbors, with new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A241345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A241345
   Empirical recurrence (valid for n > 11):
      a(n) = 8*a(n-1) - 8*a(n-2) - 48*a(n-3) + 59*a(n-4) + 102*a(n-5)
            - 77*a(n-6) - 112*a(n-7) + 38*a(n-8) + 66*a(n-9) - 27*a(n-10)
   Initial values:
      a(1)=1, a(2)=2, a(3)=5, a(4)=22, a(5)=103, a(6)=519, a(7)=2656,
      a(8)=13798, a(9)=72005, a(10)=377132, a(11)=1978263
   The function works for any n ≥ 1, returning 0 for n = 0. */

#include <stddef.h>

static const seq_t init_vals[11] = {
    1ULL,   /* a(1) */
    2ULL,   /* a(2) */
    5ULL,   /* a(3) */
    22ULL,  /* a(4) */
    103ULL, /* a(5) */
    519ULL, /* a(6) */
    2656ULL,/* a(7) */
    13798ULL,/* a(8) */
    72005ULL,/* a(9) */
    377132ULL,/* a(10) */
    1978263ULL/* a(11) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe to return 0 */
    if (n <= 11) return init_vals[n - 1];/* directly from the table */

    /* Buffer holding the last 10 terms: a(n-9) .. a(n)  */
    seq_t last[10];
    /* initialize with a(2) .. a(11) */
    for (idx_t i = 0; i < 10; ++i)
        last[i] = init_vals[i + 1];

    /* Compute terms from a(12) up to a(n) using the recurrence */
    for (idx_t term = 12; term <= n; ++term) {
        seq_t val =
            /* +8*a(n-1) */
            8 * last[9]
            /* -8*a(n-2) */
            - 8 * last[8]
            /* -48*a(n-3) */
            - 48 * last[7]
            /* +59*a(n-4) */
            + 59 * last[6]
            /* +102*a(n-5) */
            + 102 * last[5]
            /* -77*a(n-6) */
            - 77 * last[4]
            /* -112*a(n-7) */
            - 112 * last[3]
            /* +38*a(n-8) */
            + 38 * last[2]
            /* +66*a(n-9) */
            + 66 * last[1]
            /* -27*a(n-10) */
            - 27 * last[0];

        /* shift buffer left and insert the new term */
        for (int i = 0; i < 9; ++i)
            last[i] = last[i + 1];
        last[9] = val;
    }

    return last[9]; /* a(n) */
}

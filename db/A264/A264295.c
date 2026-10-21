/*
 * A264295  Number of (n+1)X(3+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change 0,1 0,-2 1,0 or -1,0.
 * https://oeis.org/A264295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2) - 34*a(n-3) + 79*a(n-4) - 103*a(n-5)
 *            + 60*a(n-6) - 220*a(n-7) + 610*a(n-8) - 142*a(n-9) -
 *            1276*a(n-10) + 1276*a(n-11) + 142*a(n-12) - 610*a(n-13) +
 *            220*a(n-14) - 60*a(n-15) + 103*a(n-16) - 79*a(n-17) + 34*a(n-18)
 *            - 2*a(n-19) - 5*a(n-20) + a(n-21)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[21] = {
    /* a(1) .. a(21) */
    (seq_t)5,
    (seq_t)8,
    (seq_t)89,
    (seq_t)192,
    (seq_t)1413,
    (seq_t)4352,
    (seq_t)24781,
    (seq_t)90112,
    (seq_t)449665,
    (seq_t)1781256,
    (seq_t)8340733,
    (seq_t)34594368,
    (seq_t)156594265,
    (seq_t)665254400,
    (seq_t)2958805225,
    (seq_t)12729812160,
    (seq_t)56099851853,
    (seq_t)242965239816,
    (seq_t)1065598885233,
    (seq_t)4631110326400,
    (seq_t)20260075655037
};

static const int coeffs[21] = {
    5, 2, -34, 79, -103, 60, -220, 610, -142, -1276,
    1276, 142, -610, 220, -60, 103, -79, 34, -2, -5, 1
};

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n <= 21) {
        return init_vals[(size_t)(n - 1)];
    }

    /* Buffer storing the last 21 values needed for the recurrence. */
    static seq_t last[21];
    /* Load initial values once */
    for (size_t i = 0; i < 21; ++i) {
        last[i] = init_vals[i];
    }

    /* Compute values up to n */
    for (idx_t k = 22; k <= n; ++k) {
        seq_t pos = 0, neg = 0;
        for (int t = 0; t < 21; ++t) {
            int c = coeffs[t];
            seq_t term = last[20 - t] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0) {
                pos += term;
            } else {
                neg += term;
            }
        }
        seq_t next = pos - neg;  /* final result is guaranteed non‑negative */

        /* Shift buffer left and store new value */
        for (int i = 0; i < 20; ++i) last[i] = last[i + 1];
        last[20] = next;
    }

    return last[20];
}

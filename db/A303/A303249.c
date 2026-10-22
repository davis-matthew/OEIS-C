/*
 * A303249  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) + 7*a(n-3) -61*a(n-4) -78*a(n-5)
 *            +115*a(n-6) +26*a(n-7) -126*a(n-8) -300*a(n-9) +380*a(n-10)
 *            -80*a(n-11) +144*a(n-12) -32*a(n-13) +128*a(n-14) for n ≥ 15,
 *            with a(1)=1, a(2)=14, a(3)=49, a(4)=250, a(5)=1183, a(6)=5918,
 *            a(7)=28680, a(8)=141255, a(9)=691968, a(10)=3391358,...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Base terms (indices 0..14, index 0 unused) */
static const seq_t base_vals[15] = {
    0ULL,      /* unused */
    1ULL, 14ULL, 49ULL, 250ULL, 1183ULL, 5918ULL,
    28680ULL, 141255ULL, 691968ULL, 3391358ULL,
    16627204ULL, 81517689ULL, 399591042ULL, 1958997017ULL
};

/* Coefficients of the 14‑term recurrence */
static const long long coeffs[14] = {
    4LL, 6LL, 7LL, -61LL, -78LL, 115LL, 26LL,
    -126LL, -300LL, 380LL, -80LL, 144LL, -32LL, 128LL
};

seq_t a(idx_t n)
{
    if (n <= 14)
        return base_vals[(size_t)n];

    /* last[i] holds a(n-i-1), i=0..13 (last[0]=a(n-1)) */
    seq_t last[14];
    for (idx_t i = 0; i < 14; ++i)
        last[i] = base_vals[14 - i];

    for (idx_t cur = 15; cur <= n; ++cur) {
        seq_t next = 0;
        for (idx_t i = 0; i < 14; ++i) {
            long long c = coeffs[i];
            if (c >= 0)
                next += last[i] * (seq_t)c;
            else
                next -= last[i] * (seq_t)(-c);
        }
        /* Shift to keep the window of the last 14 values */
        for (idx_t i = 13; i > 0; --i)
            last[i] = last[i - 1];
        last[0] = next;
    }
    return last[0];   /* a(n) */
}

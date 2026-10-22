/*
 * A316707  For any n >= 0 with base-5 expansion Sum_{k=0..w} d_k * 5^k, let
 *          f(n) = Sum_{k=0..w} [d_k > 0] * (2 + i)^k * i^(d_k - 1) (where []
 *          is an Iverson bracket and i denotes the imaginary unit); a(n)
 *          equals the square of the modulus of f(n).
 * https://oeis.org/A316707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A316707
    a(n) = |f(n)|^2,  where
    f(n) = Σ_{k≥0, d_k>0} (2+i)^k · i^(d_k-1)
    with d_k the base‑5 digits of n.
    a(5*n) = 5·a(n),  a(5^k) = 5^k,  a(0)=0. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* Extract base‑5 digits. */
    uint8_t digs[64];
    int len = 0;
    idx_t t = n;
    while (t) {
        digs[len++] = t % 5;
        t /= 5;
    }
    /* Compute f(n) via iterative recurrence:
       f = (2+i)·f + term(d).  Use signed 128‑bit for safety. */
    __int128 real = 0, imag = 0;
    for (int i = len - 1; i >= 0; --i) {
        /* multiply f by (2+i) */
        __int128 new_real = 2 * real - imag;
        __int128 new_imag = real + 2 * imag;
        real = new_real;
        imag = new_imag;

        /* add term for current digit */
        uint8_t d = digs[i];
        if (d > 0) {
            switch (d - 1) {
                case 0: real += 1;          break; /* i^0  = 1   */
                case 1: imag += 1;          break; /* i^1  = i   */
                case 2: real -= 1;          break; /* i^2  = -1  */
                case 3: imag -= 1;          break; /* i^3  = -i  */
            }
        }
    }
    /* modulus squared */
    seq_t r2 = (seq_t)(real * real + imag * imag);
    return r2;
}

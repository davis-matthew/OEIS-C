/*
 * A385826  a(n) = Sum_{k=0..n} (binomial(n, k) mod 7).
 * https://oeis.org/A385826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n, k) mod 7)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned char smallC[7][7] = {
    /*  0 */ {1, 0, 0, 0, 0, 0, 0},
    /*  1 */ {1, 1, 0, 0, 0, 0, 0},
    /*  2 */ {1, 2, 1, 0, 0, 0, 0},
    /*  3 */ {1, 3, 3, 1, 0, 0, 0},
    /*  4 */ {1, 4, 6, 4, 1, 0, 0},
    /*  5 */ {1, 5, 3, 3, 5, 1, 0},
    /*  6 */ {1, 6, 1, 6, 1, 6, 1}
};

/* Compute C(n, k) mod 7 using Lucas's theorem */
static unsigned char binom_mod7(idx_t n, idx_t k)
{
    unsigned char res = 1;
    while (n || k) {
        unsigned ni = n % 7;
        unsigned ki = k % 7;
        if (ki > ni)
            return 0;
        res = (unsigned char)((res * smallC[ni][ki]) % 7);
        n /= 7;
        k /= 7;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; ; ++k) {
        unsigned char v = binom_mod7(n, k);
        sum += (seq_t)v;
        if (k == n)
            break;
    }
    return sum;
}

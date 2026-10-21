/*
 * A250896  Number of (n+1) X (6+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-8787 + 3389*2^n + 169*3^(3+n) + (14186-99*2^(4+n))*n +
 *            36*(145+33*2^(1+n))*n^2) / 4 (for n > 2), a(1)=10875, a(2)=31215
 * Range:     u8 none, u16 n <= 2, u32 n <= 13, u64 n <= 33, u128 n <= 73,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Offset 1 */
    if (n == 1) return 10875;
    if (n == 2) return 31215;

    /* Use a 128‑bit intermediate when seq_t is small, otherwise operate
       directly with seq_t (which may be arbitrary‑precision). */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit arithmetic */
        typedef unsigned __int128 u128;
        u128 n128 = (u128)n;

        /* Powers of 2 using shifts */
        u128 pow2_n   = ((u128)1) << (unsigned long)n;
        u128 pow2_1n  = ((u128)1) << (unsigned long)(n + 1);
        u128 pow2_4n  = ((u128)1) << (unsigned long)(n + 4);

        /* 3^(n+3) – fast exponentiation in 128‑bit */
        u128 exp = n + 3;
        u128 base = 3;
        u128 pow3_n3 = 1;
        while (exp) {
            if (exp & 1) pow3_n3 *= base;
            base *= base;
            exp >>= 1;
        }

        /* n^2 in 128‑bit */
        u128 n_sq = n128 * n128;

        /* Individual terms (all as u128) */
        u128 termA = ((u128)3389) * pow2_n;
        u128 termB = ((u128)169) * pow3_n3;
        u128 termC1 = ((u128)14186) * n128;
        u128 termC2 = ((u128)99) * pow2_4n * n128;
        u128 termC = termC1 - termC2;
        u128 partD = ((u128)145) + ((u128)33) * pow2_1n;
        u128 termD = ((u128)36) * partD * n_sq;

        /* Numerator */
        u128 numerator = termA + termB + termC + termD - (u128)8787;

        /* Divide by 4 (shift right by 2 bits) */
        u128 result = numerator >> 2;

        return (seq_t)result;
    } else {
        /* Arbitrary‑precision path – use seq_t operations directly. */
        seq_t two = (seq_t)2;
        seq_t three = (seq_t)3;

        seq_t pow2_n   = seq_pow(two, n);
        seq_t pow2_1n  = seq_pow(two, n + 1);
        seq_t pow2_4n  = seq_pow(two, n + 4);
        seq_t pow3_n3  = seq_pow(three, n + 3);

        seq_t n_seq = (seq_t)n;
        seq_t n_sq  = n_seq * n_seq;

        seq_t termA = 3389 * pow2_n;
        seq_t termB = 169 * pow3_n3;

        seq_t termC1 = 14186 * n_seq;
        seq_t termC2 = 99 * pow2_4n * n_seq;
        seq_t termC = termC1 - termC2;

        seq_t partD = 145 + 33 * pow2_1n;
        seq_t termD = 36 * partD * n_sq;

        seq_t numerator = termA + termB + termC + termD - 8787;
        return numerator / 4;
    }
}

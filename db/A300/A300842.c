/*
 * A300842  Number of 4-cycles in the n-Keller graph.
 * https://oeis.org/A300842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n - 3) * (21*(-2^(4*n + 1) - 3^n + 4^n + 3^n*4^(n +
 *            1) - 3^n*4^(2*n + 1) + 6^(2*n + 1) - 2*9^n + 21^n - 4*27^n +
 *            64^n) + (-63 - 14*3^n + 21*4^(n + 1) + 7*3^(n + 2)*4^(n + 1) -
 *            21*4^(2*n + 1) + 72*7^n - 28*9^(n + 1))*n + 7*(3 + 9*2^(2*n + 1)
 *            - 28*3^n)*n^2 - 84*n^3) / 21
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Computes a(n) = number of 4‑cycles in the n‑Keller graph. */
seq_t a(idx_t n)
{
    /* For n = 1 the sequence value is 0. */
    if (n == 1) return (seq_t)0;

    seq_t sn = (seq_t)n;

    /* ----- Pre‑computed powers ----- */
    seq_t pow2_4n1   = big_pow((seq_t)2,     (seq_t)(4*n + 1));   /* 2^(4n+1)      */
    seq_t pow3n      = big_pow((seq_t)3,     sn);                 /* 3^n           */
    seq_t pow4n      = big_pow((seq_t)4,     sn);                 /* 4^n           */
    seq_t pow4n1     = big_pow((seq_t)4,     (seq_t)(n + 1));     /* 4^(n+1)       */
    seq_t pow4_2n1   = big_pow((seq_t)4,     (seq_t)(2*n + 1));   /* 4^(2n+1)      */
    seq_t pow6_2n1   = big_pow((seq_t)6,     (seq_t)(2*n + 1));   /* 6^(2n+1)      */
    seq_t pow9n      = big_pow((seq_t)9,     sn);                 /* 9^n           */
    seq_t pow21n     = big_pow((seq_t)21,    sn);                 /* 21^n          */
    seq_t pow27n     = big_pow((seq_t)27,    sn);                 /* 27^n          */
    seq_t pow64n     = big_pow((seq_t)64,    sn);                 /* 64^n          */
    seq_t pow3_n2    = big_pow((seq_t)3,     (seq_t)(n + 2));     /* 3^(n+2)       */
    seq_t pow9_np1   = big_pow((seq_t)9,     (seq_t)(n + 1));     /* 9^(n+1)       */
    seq_t pow7n      = big_pow((seq_t)7,     sn);                 /* 7^n           */
    seq_t pow2_2np1  = big_pow((seq_t)2,     (seq_t)(2*n + 1));   /* 2^(2n+1)      */

    /* ----- Inside the 21*... parentheses ----- */
    seq_t inside = -pow2_4n1           // -2^(4n+1)
                - pow3n               // -3^n
                + pow4n               // +4^n
                + (pow3n * pow4n1)    // +3^n * 4^(n+1)
                - (pow3n * pow4_2n1) // -3^n * 4^(2n+1)
                + pow6_2n1            // +6^(2n+1)
                - ((seq_t)2 * pow9n)  // -2*9^n
                + pow21n              // +21^n
                - ((seq_t)4 * pow27n) // -4*27^n
                + pow64n;             // +64^n

    /* Multiply by 21 */
    seq_t part0 = inside * (seq_t)21;

    /* ----- Second part ( ... ) * n ----- */
    seq_t part1 = -(seq_t)63
                - ((seq_t)14 * pow3n)
                + ((seq_t)21 * pow4n1)
                + ((seq_t)7 * (pow3_n2 * pow4n1))
                - ((seq_t)21 * pow4_2n1)
                + ((seq_t)72 * pow7n)
                - ((seq_t)28 * pow9_np1);
    part1 *= sn;                            /* * n */

    /* ----- Third part: 7*(3 + 9*2^(2*n+1) - 28*3^n)*n^2 ----- */
    seq_t inner3 = (seq_t)3
                + ((seq_t)9 * pow2_2np1)
                - ((seq_t)28 * pow3n);
    seq_t part2 = ((seq_t)7 * inner3) * (sn * sn);

    /* ----- Fourth part: -84*n^3 ----- */
    seq_t part3 = (seq_t)84 * (sn * sn * sn);    /* * n^3, will be subtracted */

    /* ----- Numerator ----- */
    seq_t numerator = part0 + part1 + part2 - part3;

    /* ----- Divide by 21 ----- */
    seq_t divstep = numerator / (seq_t)21;

    /* ----- Multiply by 2^(2*n-3) ----- */
    seq_t pref = big_pow((seq_t)2, (seq_t)(2*n - 3));
    seq_t result = pref * divstep;

    return result;
}

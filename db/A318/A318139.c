/*
 * A318139  The 10-adic integer e = ...3455904677 satisfying e^2 + 1 = f, f^2
 *          + 1 = a, a^2 + 1 = b, b^2 + 1 = c, c^2 + 1 = d, and d^2 + 1 = e.
 * https://oeis.org/A318139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(n) is the nth digit (least significant first) of the unique
 *            10-adic integer e * that satisfies the cycle * * e^2 + 1 = f *
 *            f^2 + 1 = a * a^2 + 1 = b * b^2 + 1 = c * c^2 + 1 = d * d^2 + 1
 *            = e * * The digits are obtained by iteratively lifting the
 *            congruences modulo 10^k. * For each digit posit...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static state: digits of the six variables and current carry values. */
    static uint8_t  *e_digs = NULL, *f_digs = NULL, *a_digs = NULL;
    static uint8_t  *b_digs = NULL, *c_digs = NULL, *d_digs = NULL;
    static size_t   len = 0;   /* number of digits computed (next free index)  */
    static size_t   cap = 0;   /* allocated capacity                               */
    static uint32_t carry_e = 0, carry_f = 0, carry_a = 0;
    static uint32_t carry_b = 0, carry_c = 0, carry_d = 0;

    /* Units digits of the six variables (constants). */
    const uint8_t u_e = 7, u_f = 0, u_a = 1, u_b = 2, u_c = 5, u_d = 6;

    /* Initialise on first call. */
    if (len == 0) {
        cap = 1;
        e_digs = (uint8_t *)malloc(cap);
        f_digs = (uint8_t *)malloc(cap);
        a_digs = (uint8_t *)malloc(cap);
        b_digs = (uint8_t *)malloc(cap);
        c_digs = (uint8_t *)malloc(cap);
        d_digs = (uint8_t *)malloc(cap);

        /* Digit at position 0 of the six variables. */
        e_digs[0] = 7;  f_digs[0] = 0;
        a_digs[0] = 1;  b_digs[0] = 2;
        c_digs[0] = 5;  d_digs[0] = 6;

        /* Carry into position 1 after adding 1 to X^2. */
        carry_e = (u_e * u_e + 1) / 10;   /* (49+1)/10 = 5 */
        carry_f = (u_f * u_f + 1) / 10;   /* (0+1)/10  = 0 */
        carry_a = (u_a * u_a + 1) / 10;   /* (1+1)/10  = 0 */
        carry_b = (u_b * u_b + 1) / 10;   /* (4+1)/10  = 0 */
        carry_c = (u_c * u_c + 1) / 10;   /* (25+1)/10 = 2 */
        carry_d = (u_d * u_d + 1) / 10;   /* (36+1)/10 = 3 */

        len = 1;   /* we have index 0 ready */
    }

    /* Extend the precomputed digits up to index n. */
    while (len <= (size_t)n) {
        size_t k = len;        /* index of the new digit to compute
                                 (positions are 0‑indexed)          */

        /* Ensure capacity.  Use doubling strategy. */
        if (len >= cap) {
            size_t newcap = cap ? cap * 2 : 1;
            e_digs = (uint8_t *)realloc(e_digs, newcap);
            f_digs = (uint8_t *)realloc(f_digs, newcap);
            a_digs = (uint8_t *)realloc(a_digs, newcap);
            b_digs = (uint8_t *)realloc(b_digs, newcap);
            c_digs = (uint8_t *)realloc(c_digs, newcap);
            d_digs = (uint8_t *)realloc(d_digs, newcap);
            cap = newcap;
        }

        /* Compute sum_known for each variable:
           sum of products X_i*X_{k-i} over i=1..k-1. */
        uint32_t sum_e = 0, sum_f = 0, sum_a = 0, sum_b = 0, sum_c = 0, sum_d = 0;

        for (size_t i = 1; i < k; ++i) {
            sum_e += (uint32_t)e_digs[i] * e_digs[k - i];
            sum_f += (uint32_t)f_digs[i] * f_digs[k - i];
            sum_a += (uint32_t)a_digs[i] * a_digs[k - i];
            sum_b += (uint32_t)b_digs[i] * b_digs[k - i];
            sum_c += (uint32_t)c_digs[i] * c_digs[k - i];
            sum_d += (uint32_t)d_digs[i] * d_digs[k - i];
        }

        /* R_X = (carry_X + sum_known_X) % 10.  No +1 because position k≥1. */
        uint8_t R_e = (carry_e + sum_e) % 10;
        uint8_t R_f = (carry_f + sum_f) % 10;
        uint8_t R_a = (carry_a + sum_a) % 10;
        uint8_t R_b = (carry_b + sum_b) % 10;
        uint8_t R_c = (carry_c + sum_c) % 10;
        uint8_t R_d = (carry_d + sum_d) % 10;

        /* Compute the new digits using the derived linear chain. */
        uint8_t a_k = R_f;                                       /* a_k = R_f */
        uint8_t b_k = (2 * a_k + R_a) % 10;                     /* b_k */
        uint8_t c_k = (4 * b_k + R_b) % 10;                     /* c_k */
        uint8_t d_k = R_c;                                       /* d_k = R_c */
        uint8_t e_k = (2 * d_k + R_d) % 10;                     /* e_k */
        uint8_t f_k = (4 * e_k + R_e) % 10;                     /* f_k */

        /* Store the new digits. */
        e_digs[k] = e_k;
        f_digs[k] = f_k;
        a_digs[k] = a_k;
        b_digs[k] = b_k;
        c_digs[k] = c_k;
        d_digs[k] = d_k;

        /* Update carries for the next position (position k+1). */
        carry_e = (carry_e + sum_e + 2 * u_e * e_k) / 10;
        carry_f = (carry_f + sum_f + 2 * u_f * f_k) / 10;
        carry_a = (carry_a + sum_a + 2 * u_a * a_k) / 10;
        carry_b = (carry_b + sum_b + 2 * u_b * b_k) / 10;
        carry_c = (carry_c + sum_c + 2 * u_c * c_k) / 10;
        carry_d = (carry_d + sum_d + 2 * u_d * d_k) / 10;

        ++len;   /* one more digit is now available */
    }

    /* Return the requested digit of e (sequence a). */
    return (seq_t)e_digs[(size_t)n];
}

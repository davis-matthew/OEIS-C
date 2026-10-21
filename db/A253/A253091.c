/*
 * A253091  List of ternary words obtained by expanding (1+2x)^n mod 3 and
 *          reading the coefficients starting with the constant term.
 * https://oeis.org/A253091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer whose digits are the coefficients of
 *            (1+2x)^n modulo 3, read from the constant term upwards
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const unsigned char binom_small[3][3] = {
    {1,0,0},               /* n = 0 : binom(0,0) = 1 */
    {1,1,0},               /* n = 1 : binom(1,0)=1 , binom(1,1)=1 */
    {1,2,1}                /* n = 2 : binom(2,0)=1 , binom(2,1)=2 , binom(2,2)=1 */
};

/* Compute binomial coefficient modulo 3 using Lucas' theorem */
static unsigned char binom_mod3(idx_t N, idx_t K)
{
    if (K > N) return 0;
    unsigned char res = 1;
    while (N || K) {
        int n_dig = N % 3;
        int k_dig = K % 3;
        if (k_dig > n_dig) return 0;
        res = (res * binom_small[n_dig][k_dig]) % 3;
        N /= 3;
        K /= 3;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    /* iterate over k = 0 .. n, coefficients from constant term upwards */
    for (idx_t k = 0; k <= n; ++k) {
        unsigned char bin_coeff = binom_mod3(n, k);
        unsigned char pow2k = (k & 1) ? 2 : 1;          /* 2^k mod 3 */
        unsigned char digit = (bin_coeff * pow2k) % 3;   /* coefficient mod 3 */
        result = result * (seq_t)10 + (seq_t)digit;     /* build decimal number */
    }
    return result;
}

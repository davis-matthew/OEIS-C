/*
 * A387680  Number of length n words on the alphabet {a,b,c,d,e} that do not
 *          contain exactly one a and exactly one b.
 * https://oeis.org/A387680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - n*(n-1)*3^(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the small cases directly to avoid negative exponents. */
    if (n == 0) return 1;
    if (n == 1) return 5;

    /* Compute 5^n and 3^(n-2) using the arbitrary‑precision big_pow routine. */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 2));

    /* Compute the subtraction term n*(n-1)*3^(n-2). */
    seq_t mul = ((seq_t)n * (seq_t)(n - 1)) * pow3;

    /* Result is 5^n - n*(n-1)*3^(n-2). */
    return pow5 - mul;
}

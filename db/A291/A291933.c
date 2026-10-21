/*
 * A291933  a(1)=2; a(n) is the sum of prime(n) and the last digit of previous
 *          term.
 * https://oeis.org/A291933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=2; a(n)=prime(n)+ (a(n-1) mod 10) for n>1
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A291933 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t term = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = (seq_t)get_prime(i);   /* prime(i) */
        if (i == 1) {                     /* a(1) = 2 */
            term = p;
        } else {
            seq_t last_digit = term % 10;
            term = p + last_digit;
        }
    }
    return term;
}

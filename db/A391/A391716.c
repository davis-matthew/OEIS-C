/*
 * A391716  Let f be an additive function with f(2^e) = (e + e mod 2)/2,
 *          f(p^e) = e*(p-1)/2, p an odd prime. a(n) is the number of
 *          iterations of f(n) required to reach 0.
 * https://oeis.org/A391716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1555, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A391716: number of iterations of the additive function
   f(n) until reaching 0. The function f is defined by
   f(2^e) = (e + e mod 2)/2,
   f(p^e) = e*(p-1)/2 for odd primes p.
   a(n) is the number of times f must be applied to n to obtain 0.
   Implementation uses simple prime‑factorisation for each iteration. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/add_f.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but guard anyway */
    seq_t count = 0;
    while (n != 0) {
        n = add_f(n);
        count++;
    }
    return count;
}

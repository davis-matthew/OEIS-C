/*
 * A303811  Least k such that A006666(k)/A006667(k) = prime(n).
 * https://oeis.org/A303811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A303811
   a(n) = 5 * 2^(prime(n)-4)  for n > 2
   a(1) = 159, a(2) = 6   (special cases) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t k) */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)159;
    if (n == 2) return (seq_t)6;

    /* nth prime (1‑based) */
    uint32_t p = get_prime(n);
    /* exponent for 2 is p-4 */
    idx_t exp = (idx_t)(p - 4u);

    seq_t val = pow2(exp);   /* 2^exp */
    val *= (seq_t)5;         /* multiply by 5 */
    return val;
}

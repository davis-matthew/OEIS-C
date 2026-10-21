/*
 * A273801  Numbers n for which n = (x - phi(x)) * (y - phi(y)), where n = x +
 *          y and x - phi(x) is the Euler cototient function of x.
 * https://oeis.org/A273801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * (prime(n+1) + 1)
 * Range:     u8 n <= 17, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following header provides the get_prime(k) utility that returns
 * the k‑th prime (1‑based). It is part of the utilities bundle.
 */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Use the (n+1)‑th prime in the OEIS formula. */
    idx_t k = n + 1;                     /* 1‑based index of the desired prime */
    seq_t p = (seq_t)get_prime(k);       /* convert prime to seq_t */
    seq_t res = ((seq_t)4) * (p + 1);    /* apply the formula */
    return res;
}

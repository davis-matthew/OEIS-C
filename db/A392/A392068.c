/*
 * A392068  a(n) = ((n*p)^p - 1)/(n*p-1) where p = prime(n).
 * https://oeis.org/A392068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n*p)^p - 1)/(n*p-1) where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, defensive */
    uint32_t p = get_prime(n);           /* n-th prime, 1‑indexed */
    seq_t np = (seq_t)n * (seq_t)p;      /* n * p  */
    seq_t val = seq_pow(np, p);          /* (n*p)^p  */
    return (val - (seq_t)1) / (np - (seq_t)1);
}

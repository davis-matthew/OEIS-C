/*
 * A242089  Number of triples (a,b,c) with 0 < a < b < c < p and a + b + c ==
 *          0 mod p, where p = prime(n).
 * https://oeis.org/A242089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * round((prime(n)-3)^2 / 12)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n starts from 1 */
    seq_t p = (seq_t)get_prime(n);   /* prime(n) */
    seq_t x = p - 3;                  /* p-3 */
    /* Use 64‑bit intermediate to avoid overflow for 32‑bit builds */
    uint64_t x64 = (uint64_t)x;
    uint64_t num = x64 * x64 + 6;     /* (p-3)^2 + 6 */
    seq_t res = ((seq_t)(num / 12)) * 2;   /* round and double */
    return res;
}

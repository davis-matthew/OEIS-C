/*
 * A341410  a(n) = (Sum_{k=1..4} k^n) mod n.
 * https://oeis.org/A341410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1^n + 2^n + 3^n + 4^n) mod n
 * Range:     u8 n <= 277, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t mod = (seq_t)n;
    seq_t sum = 0;

    sum = (sum + mod_pow((seq_t)1, n, mod)) % mod;
    sum = (sum + mod_pow((seq_t)2, n, mod)) % mod;
    sum = (sum + mod_pow((seq_t)3, n, mod)) % mod;
    sum = (sum + mod_pow((seq_t)4, n, mod)) % mod;

    return sum;
}

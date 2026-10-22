/*
 * A368118  a(n) = ceiling(1/p(n)) if p(n) > 0 otherwise 0, where p(n) =
 *          2*sin(Pi * Gamma(n) / n).
 * https://oeis.org/A368118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( n / (2π) ) for prime n; 0 otherwise
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* High‑precision rational approximation of 1/(2π).  */
static const unsigned long long NUM = 1591549430918953ULL; /* ≈ 1/(2π) * 10^15 */
static const unsigned long long DEN = 10000000000000000ULL;/* 10^16       */

seq_t a(idx_t n)
{
    if (!is_prime(n))
        return 0;

    __int128 prod = (__int128)n * NUM;
    seq_t val = (seq_t)((prod + DEN - 1) / DEN); /* ceil(prod/DEN) */
    return val;
}

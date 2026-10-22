/*
 * A301316  a(n) = ((n-1)! + 1) mod n^2.
 * https://oeis.org/A301316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)! + 1) mod n^2
 * Range:     u8 n <= 25, u16 n <= 280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure that the sequence is defined for n>=1.
       For n==1 we have (0!+1) mod 1 = 0. */
    if (n <= 1) {
        return 0;
    }

    /* Compute m = n^2 in a 128‑bit type to avoid overflow during the
       factorial computation.  A 128‑bit type is available on GCC/Clang and
       also convertible to the build’s `seq_t` (which may be
       unsigned __int128 or an arbitrary precision integer). */
    unsigned __int128 mod = (unsigned __int128)n * (unsigned __int128)n;

    unsigned __int128 fact = 1;
    /* Multiply 2 * 3 * ... * (n-1) modulo mod.  Using the 128‑bit
       intermediate keeps the product from overflowing before the
       modulo operation. */
    for (idx_t i = 2; i < n; ++i) {
        fact = (fact * i) % mod;
    }

    unsigned __int128 result = (fact + 1) % mod;
    return (seq_t)result;   /* cast back to the sequence type */
}

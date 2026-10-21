/*
 * A293262  a(n) = floor(prime(n)*(e-2)).
 * https://oeis.org/A293262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(prime(n)*(e-2))
 * Range:     u8 n <= 71, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* constants: e-2 ≈ 0.718281828459045 ...  */
    /* 0.718281828459045 = 718281828459045 / 10^15  */
    static const unsigned __int128 NUM = 718281828459045ULL;
    static const unsigned __int128 DEN = 1000000000000000ULL;

    /* get the nth prime; n is guaranteed ≥ 1 in the OEIS specification */
    uint32_t p = get_prime(n);              /* prime(n) as 32‑bit value */

    /* compute floor(p * NUM / DEN) safely using 128‑bit arithmetic */
    unsigned __int128 prod = (unsigned __int128)p * NUM;
    unsigned __int128 value = prod / DEN;

    return (seq_t)value;
}

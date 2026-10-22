/*
 * A380332  a(n) = number of primes between n^2 and n^4.
 * https://oeis.org/A380332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi(n^4) - PrimePi(n^2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* compute n^2 and n^4 using seq_t to keep arbitrary‑precision */
    seq_t sq     = (seq_t)n * (seq_t)n;   /* n^2      */
    seq_t sq_pow2 = sq * sq;              /* n^4      */

    /* prime_pi expects an idx_t, so cast from seq_t (value fits in uint64_t) */
    return prime_pi((idx_t)sq_pow2) - prime_pi((idx_t)sq);
}

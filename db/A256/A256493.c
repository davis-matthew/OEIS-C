/*
 * A256493  Number of factorizations of m^3 into at most 3 factors, where m is
 *          a product of exactly n distinct primes.
 * https://oeis.org/A256493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (10^n + 3*2^n + 2) / 6
 *
 * The implementation uses arbitrary‑precision arithmetic when
 * seq_t is larger than 64 bits.  For the 64‑bit build we employ
 * unsigned __int128 so that intermediate values up to 10^38
 * (n = 38) fit comfortably and the final result is cast back
 * to seq_t.
 *
 * The result fits in seq_t for all n for which the sequence
 * value itself fits into it; for larger n the 64‑bit build
 * would overflow and is not required to produce a result.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* If seq_t is 64‑bit, use 128‑bit intermediates. */
    if (sizeof(seq_t) == sizeof(uint64_t))
    {
        unsigned __int128 pow10 = 1;
        unsigned __int128 pow2  = 1;

        for (idx_t i = 0; i < n; ++i) pow10 *= 10;
        for (idx_t i = 0; i < n; ++i) pow2  *= 2;

        unsigned __int128 total = pow10 + (pow2 * 3) + 2;
        return (seq_t)(total / 6);
    }

    /* General (arbitrary‑precision) case */
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    seq_t pow2  = big_pow((seq_t)2,  (seq_t)n);

    seq_t sum = pow10 + mul_small(pow2, 3) + (seq_t)2;
    return sum / (seq_t)6;
}

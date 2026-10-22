/*
 * A312620  Coordination sequence Gal.6.256.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A312620 – Coordination sequence Gal.6.256.1
 *
 *  Formula:  a(n) = 5*n - ⌊n/3⌋ + d(n)
 *
 *  where d(n) is a periodic correction:
 *
 *        d(n) =  1, –1, –2, 0, 0, 0, 0, –1, –1, 0, 1, 0   for 0 ≤ n < 12
 *        d(n) =   0, –1, –2, 0, 0, 0, 0, –1, –1, 0, 1, 0   for n ≥ 12,
 *                repeating every 12 values.
 *
 *  The implementation uses only integer arithmetic, works for any
 *  seq_t / idx_t supplied by the build environment, and compiles
 *  under both C11 and C++17.
 */

#include <stddef.h>
#include <stdint.h>

static const int8_t _d_pre12[12] = {  1, -1, -2,  0,  0,  0,  0, -1, -1,  0,  1,  0 };
static const int8_t _d_cycle[12] = {  0, -1, -2,  0,  0,  0,  0, -1, -1,  0,  1,  0 };

seq_t a(idx_t n)
{
    /* 5*n – floor(n/3) computed in an unsigned 64‑bit intermediate. */
    unsigned long long base = (unsigned long long)5 * (unsigned long long)n;
    unsigned long long div3 = (unsigned long long)(n / 3ull);
    unsigned long long val  = base - div3;

    /* Select the appropriate periodic correction d(n). */
    int8_t d;
    if (n < 12ull)
        d = _d_pre12[(size_t)n];
    else
        d = _d_cycle[(size_t)((n - 12ull) % 12ull)];

    /* Combine the components.  The final result fits into seq_t
     * for all values that are represented by seq_t. */
    long long result = (long long)val + (long long)d;
    return (seq_t)result;
}

/*
 * A248469  a(n) = floor((2*n^2)/e).
 * https://oeis.org/A248469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^2)/e)
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t SCALE   = 1000000000000000ULL;      /* 10^15   */
static const uint64_t E_SCALE = 2718281828459045ULL;      /* floor(e*10^15) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* 0 is not defined for the sequence, but safe */

    unsigned long long nn   = (unsigned long long)n;      /* n */
    unsigned long long sq   = nn * nn;                   /* n^2 */
    unsigned long long num  = sq * 2ULL;                 /* 2*n^2 */

    /* 2*n^2 * SCALE  (fits into 128 bits for n <= 10^4) */
    __uint128_t num_scaled = ( __uint128_t )num * SCALE;

    /* upper bound estimate using floor(e*10^15) */
    uint64_t ans = (uint64_t)( num_scaled / E_SCALE );

    /* adjust upward if (ans+1) still satisfies the inequality */
    while ( ( (__uint128_t)(ans + 1) * (E_SCALE + 1ULL) ) <= num_scaled )
        ++ans;

    /* adjust downward if ans is too large */
    while ( ( (__uint128_t)ans * (E_SCALE + 1ULL) ) > num_scaled )
        --ans;

    return (seq_t)ans;
}

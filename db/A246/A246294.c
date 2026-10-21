/*
 * A246294  Numbers k such that sin(k) < sin(k+1) > sin(k+2).
 * https://oeis.org/A246294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n-1) * 2π + π/2 - 0.5 )
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using a rational approximation of 2π and π/2 - 0.5
   with denominator 66430. The constants below are integers:
      ALPHA_NUM  = 417392  ≈ 2π * 66430
      CNUM       = 71133   ≈ (π/2 - 0.5) * 66430
   Then a(n) = floor( ((n-1)*ALPHA_NUM + CNUM) / 66430 ).  */

#include <stdint.h>

static const __int128 SCALE     = 66430;   /* denominator */
static const __int128 ALPHA_NUM = 417392;  /* 2π * 66430  */
static const __int128 CNUM      = 71133;   /* (π/2 - 0.5) * 66430 */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n=0; return 0 as a safe default */
        return 0;

    idx_t m = n - 1;
    __int128 num = (__int128)m * ALPHA_NUM + CNUM;
    seq_t result = (seq_t)(num / SCALE);
    return result;
}

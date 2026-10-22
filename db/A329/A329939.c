/*
 * A329939  Beatty sequence for cosh x, where csch x + sech x = 1 .
 * https://oeis.org/A329939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*cosh x) where cosh(x) ≈ 2.132241889149... (root
 *            of 4c⁴–8c³–4c²+4c+5=0)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _COSH_X_NUM = (seq_t)2132241889150000000ULL;  /* floor(cosh(x)*10¹⁸) */
static const seq_t _SCALE_1E18  = (seq_t)1000000000000000000ULL; /* 10¹⁸ */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow on 64‑bit builds. */
    return (seq_t)( ((unsigned __int128)n * (unsigned __int128)_COSH_X_NUM) / _SCALE_1E18 );
}

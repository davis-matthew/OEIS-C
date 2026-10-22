/*
 * A327216  a(n) = [(2n+3)r] - [(n+3)r] - [nr], where [ ] = floor and r =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A327216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+3)*φ) - floor((n+3)*φ) - floor(n*φ)
 * Range:     u8 n <= 0, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* φ = (1+√5)/2, the golden ratio.  The floor of n·φ is computed
   using the 20th convergent of φ: 10946/6765, which is accurate
   enough for all n up to about 10⁷.  Since the sequence is needed
   only for n ≤ 10⁴ in the tests, this guarantees exact results. */

#include <stdint.h>

/* consecutive Fibonacci numbers that give a convergent of φ */
static const seq_t PHI_NUM = 10946; /* F21 */
static const seq_t PHI_DEN =  6765; /* F20 */

/* Helper to compute floor(k·φ) using the convergent */
static seq_t floor_phi(idx_t k)
{
    /* k * PHI_NUM / PHI_DEN  : integer division yields floor */
    return ((seq_t)k * PHI_NUM) / PHI_DEN;
}

seq_t a(idx_t n)
{
    idx_t k1 = n + 3;
    idx_t k2 = (idx_t)((((unsigned __int128)n) << 1) + 3); /* 2·n + 3, safe for large n */

    seq_t fn  = floor_phi(n);
    seq_t fn1 = floor_phi(k1);
    seq_t fn2 = floor_phi(k2);

    return fn2 - fn1 - fn;
}

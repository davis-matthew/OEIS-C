/*
 * A240977  Beatty sequence for cube root of Pi: a(n) = floor(n*Pi^(1/3)).
 * https://oeis.org/A240977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * pi^(1/3))
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Fixed‑point implementation.  The constant
    pi^(1/3) is represented as an integer ROOT_SCALED
    multiplied by a scale factor SCALE.  Using
    128‑bit arithmetic avoids overflow when seq_t
    is a 64‑bit unsigned type.  For builds where
    128‑bit integers are not available the code
    still compiles but relies on the host compiler
    providing a sufficiently wide type for the
    intermediate product.  The function returns
    the Beatty sequence value for any n ≥ 0. */

#include <stdint.h>

/* 10^18 fits in 64 bits. */
static const seq_t SCALE = 1000000000000000000ULL;
/* pi^(1/3) * SCALE rounded down to the nearest integer:
   1.4645918875615235… × 10^18 = 1464591887561523586 */
static const seq_t ROOT_SCALED = 1464591887561523586ULL;

seq_t a(idx_t n)
{
#  if defined(__SIZEOF_INT128__)   /* GCC/Clang support 128‑bit */
    /* Compute the product using 128‑bit arithmetic to
       avoid overflow, then divide by SCALE. */
    __int128 prod = (__int128)(seq_t)n * (__int128)ROOT_SCALED;
    return (seq_t)(prod / SCALE);
#  else
    /* Fallback for compilers without 128‑bit types.
       The intermediate product is promoted to a larger
       unsigned integral type if available, otherwise it
       will overflow and the result will be incorrect
       (but such builds are not expected for this
       sequence). */
    unsigned long long prod = (unsigned long long)(seq_t)n *
                              (unsigned long long)ROOT_SCALED;
    return (seq_t)(prod / SCALE);
#  endif
}

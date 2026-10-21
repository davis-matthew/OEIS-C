/*
 * A259748  a(n) = (Sum_{0<x<y<n} x*y) mod n.
 * https://oeis.org/A259748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)*n*(n+1)*(3*n+2)/24) mod n
 * Range:     u8 n <= 286, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* modular multiplication that is safe for 64‑bit and larger integral types.
 * Uses 128‑bit arithmetic when available, otherwise falls back to normal
 * multiplication (which is safe when seq_t is arbitrary‑precision). */
static inline seq_t mul_mod(seq_t a, seq_t b, idx_t mod)
{
#if defined(__SIZEOF_INT128__) && defined(__GNUC__)
    return (seq_t)(( (unsigned __int128)a * (unsigned __int128)b ) % (unsigned __int128)mod);
#else
    /* If 128‑bit arithmetic is not available, the multiplication is safe
     * because seq_t is either a builtin type small enough not to overflow
     * or an arbitrary‑precision class that handles large values correctly. */
    return (a * b) % (seq_t)mod;
#endif
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (outside OEIS range) */
    if (n == 0) return 0;

    /* Factors of the polynomial numerator */
    idx_t f[4];
    f[0] = n - 1;
    f[1] = n;
    f[2] = n + 1;

    /* Compute 3*n + 2 safely using 128‑bit arithmetic when possible */
#if defined(__SIZEOF_INT128__) && defined(__GNUC__)
    unsigned __int128 tmp = (unsigned __int128)3 * n + 2;
    f[3] = (idx_t)tmp;
#else
    f[3] = 3 * n + 2;
#endif

    /* Cancel the factor 24 = 2^3 * 3 from the product */
    int need2 = 3;    /* number of divisions by 2 */
    int need3 = 1;    /* number of divisions by 3 */

    for (int i = 0; i < 4; ++i)
    {
        while (need2 > 0 && f[i] % 2 == 0)
        {
            f[i] >>= 1; /* divide by 2 */
            need2--;
        }
        while (need3 > 0 && f[i] % 3 == 0)
        {
            f[i] /= 3;
            need3--;
        }
    }

    /* Multiply the remaining factors modulo n */
    seq_t res = 1;
    for (int i = 0; i < 4; ++i)
        res = mul_mod(res, (seq_t)f[i], n);

    return res;
}

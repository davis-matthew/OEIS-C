/*
 * A386700  a(n) = Sum_{k=0..n} (-3)^(n-k) * binomial(3*n,k).
 * https://oeis.org/A386700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-3)^(n-k) * binomial(3*n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 18*n*(2*n - 1)*(55*n^2 - 175*n + 138)*a(n) = */
/*   (11605*n^4 - 49410*n^3 + 74243*n^2 - 46014*n + 9720)*a(n-1) + */
/*   24*(3*n - 5)*(3*n - 4)*(55*n^2 - 65*n + 18)*a(n-2).      */
#include <stdint.h>
#include <stddef.h>

/* Normal integer builds (seq_t is a fixed‑size unsigned integer).  */
#if defined(__SIZEOF_INT128__)

/* The following implementation uses 128‑bit intermediates to avoid
 * overflow for 8‑byte integer types.  It works for uint8_t, uint16_t,
 * uint32_t, uint64_t and unsigned __int128.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a_prevprev = (seq_t)1; /* a(0) */
    seq_t a_prev     = (seq_t)0; /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        __int128 ii  = (__int128)i;
        __int128 i2  = ii * ii;
        __int128 i3  = i2 * ii;
        __int128 i4  = i3 * ii;

        __int128 coeff1 = 11605*i4 - 49410*i3 + 74243*i2 - 46014*ii + 9720;
        __int128 coeff2 = 24 * (3*ii - 5) * (3*ii - 4) *
                          (55*i2 - 65*ii + 18);
        __int128 denom  = 18*ii * (2*ii - 1) *
                          (55*i2 - 175*ii + 138);

        __int128 numer = coeff1 * ( __int128)a_prev + coeff2 * ( __int128)a_prevprev;
        a_cur = (seq_t)(numer / denom); /* division is exact */
        a_prevprev = a_prev;
        a_prev = a_cur;
    }
    return a_prev;
}

#else
/* Arbitrary‑precision build: all arithmetic uses seq_t.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a_prevprev = (seq_t)1; /* a(0) */
    seq_t a_prev     = (seq_t)0; /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t ii  = (seq_t)i;
        seq_t i2  = ii * ii;
        seq_t i3  = i2 * ii;
        seq_t i4  = i3 * ii;

        seq_t coeff1 = 11605*i4 - 49410*i3 + 74243*i2 - 46014*ii + 9720;
        seq_t coeff2 = 24 * (3*ii - 5) * (3*ii - 4) *
                       (55*i2 - 65*ii + 18);
        seq_t denom  = 18*ii * (2*ii - 1) *
                       (55*i2 - 175*ii + 138);

        seq_t numer = coeff1 * a_prev + coeff2 * a_prevprev;
        a_cur = numer / denom;   /* exact division */
        a_prevprev = a_prev;
        a_prev = a_cur;
    }
    return a_prev;
}
#endif

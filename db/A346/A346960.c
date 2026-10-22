/*
 * A346960  a(0) = 0, a(1) = 1; a(n) = n * (n+1) * a(n-1) + a(n-2).
 * https://oeis.org/A346960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=1, a(n)=n*(n+1)*a(n-1)+a(n-2) */

#include <stdint.h>

#if defined(__SIZEOF_INT128__) || defined(__UINT128_TYPE__)
typedef unsigned __int128 u128;
#define HAVE_U128 1
#else
#define HAVE_U128 0
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)0; /* a(0) */
    seq_t a_nm1 = (seq_t)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
#if HAVE_U128
        u128 prod = (u128)i * (i + 1) * (u128)a_nm1;
        u128 sum = prod + (u128)a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = (seq_t)sum;
#else
        /* No u128; rely on seq_t being large enough for intermediate */
        a_nm2 = a_nm1;
        a_nm1 = (seq_t)(i * (i + 1) * a_nm1 + a_nm2);
#endif
    }

    return a_nm1;
}

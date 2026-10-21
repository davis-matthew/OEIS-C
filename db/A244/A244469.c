/*
 * A244469  a(0) = 0, thereafter, a(n) = 2^(2*n-1)*( binomial((3*n-1)/2,n) -
 *          binomial(3*n/2, n)/3 ).
 * https://oeis.org/A244469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=7.
   For n≥3:
      n*(n-1)*(6*n-7)*a(n) =
          -18*(n-1)*a(n-1) +
          12*(3*n-5)*(6*n-1)*(3*n-4)*a(n-2)  */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)7;   /* a(2) */
    seq_t a_cur = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Choose the computation method according to the size of seq_t.
           For built‑in integer seq_t (≤ 64 bits) use unsigned __int128
           to avoid overflow.  For arbitrary‑precision types use seq_t
           directly.  The compiler will optimise the dead branch away. */
        if (sizeof(seq_t) <= 8) {
            /* Use 128‑bit arithmetic for intermediate values. */
            unsigned __int128 B = 12;
            B *= (unsigned __int128)(3*i-5);
            B *= (unsigned __int128)(6*i-1);
            B *= (unsigned __int128)(3*i-4);
            B *= (unsigned __int128)a_nm2;

            unsigned __int128 C = 18;
            C *= (unsigned __int128)(i-1);
            C *= (unsigned __int128)a_nm1;

            unsigned __int128 S = B - C;

            unsigned __int128 D = (unsigned __int128)i;
            D *= (unsigned __int128)(i-1);
            D *= (unsigned __int128)(6*i-7);

            a_cur = (seq_t)(S / D);
        } else {
            /* For arbitrary‑precision seq_t, rely on its native arithmetic. */
            seq_t B = (seq_t)12;
            B *= (seq_t)(3*i-5);
            B *= (seq_t)(6*i-1);
            B *= (seq_t)(3*i-4);
            B *= a_nm2;

            seq_t C = (seq_t)18;
            C *= (seq_t)(i-1);
            C *= a_nm1;

            seq_t S = B - C;
            seq_t D = (seq_t)i;
            D *= (seq_t)(i-1);
            D *= (seq_t)(6*i-7);

            a_cur = S / D;
        }

        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_nm1;
}

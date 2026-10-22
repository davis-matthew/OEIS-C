/*
 * A335112  a(n) is the greatest k > 0 such that Sum_{j=1..n} j*k^j/(k+n) is
 *          an integer, for n > 1.
 * https://oeis.org/A335112
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*abs(((n+1)*(-n)^n+(-n)^(n+2)-1)/(n+1)^2) - n which
 *            simplifies to a(n) = n*abs_num/(n+1)^2 - n where abs_num = (n
 *            odd) ? (n^2+n+1)*n^n + 1 : (n^2+n+1)*n^n - 1. The division is
 *            exact by definition
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* offset not used below 2 */

    seq_t N = (seq_t)n;
    seq_t pow_n = big_pow(N, (seq_t)n);   /* n^n */

    seq_t coeff = N * N + N + 1;          /* n^2 + n + 1 */

#if defined(__SIZEOF_INT128__)      /* use 128‑bit for intermediates */
    unsigned __int128 term   = (unsigned __int128)coeff * (unsigned __int128)pow_n;
    unsigned __int128 abs_num;
    if (n & 1)                       /* n odd: (-1)^n = -1 */
        abs_num = term + 1;
    else
        abs_num = term - 1;

    unsigned __int128 denom   = (unsigned __int128)(N + 1) * (unsigned __int128)(N + 1);
    unsigned __int128 quotient = abs_num / denom;

    seq_t res = N * (seq_t)quotient - N;
    return res;
#else
    /* fallback – use seq_t arithmetic; may overflow for large n
       but results that fit in seq_t are correct for n <= 15. */
    seq_t term = coeff * pow_n;
    seq_t abs_num = (n & 1) ? term + 1 : term - 1;
    seq_t denom   = (N + 1) * (N + 1);
    seq_t quotient = abs_num / denom;
    seq_t res = N * quotient - N;
    return res;
#endif
}

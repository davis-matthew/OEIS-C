/*
 * A394783  Triangle read by rows: T(n,k) = n^(k-1)*(k - 1)! with 1 <= k <= n.
 * https://oeis.org/A394783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m+1, k) where * m = floor((sqrt(8*(n-1)+1)-1)/2) * k =
 *            n - m*(m+1)/2 * T(p, q) = p^(q-1)*(q-1)! * * This gives OEIS
 *            A394783
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   _floor_m, fact_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fact_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/_floor_m.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n < 1, return 0 */
    idx_t n_minus1 = n - 1;
    idx_t m = _floor_m(n_minus1);                 /* m such that T_m <= n-1 < T_{m+1} */
    idx_t t = (unsigned __int128)m * (m + 1) / 2;   /* T_m */
    idx_t k = n - t;                           /* 1 <= k <= m+1 */
    seq_t p = (seq_t)(m + 1);                  /* base */
    seq_t power = seq_pow(p, k - 1);           /* (m+1)^(k-1) */
    seq_t factorial = fact_upto(k - 1);        /* (k-1)! */
    return seq_mul(power, factorial);          /* product */
}

/*
 * A341682  Successive approximations up to 5^n for the 5-adic integer
 *          Sum_{k>=0} k!.
 * https://oeis.org/A341682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{k=0..m} k!) mod 5^n, where m = 4*(n +
 *            floor(log_5(4*n)))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, floor_log5_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/floor_log5_u64.h"

seq_t a(idx_t n)
/*  Return the nth term of A341682.  */
{
    /* Compute 5^n once; fits in seq_t for all practical n of this build. */
    seq_t mod = big_pow((seq_t)5, (seq_t)n);

    /* m = 4*(n + floor(log_5(4*n)))   (floor_log5_u64 works for uint64_t n) */
    idx_t tmp = 4 * n;
    idx_t log5 = floor_log5_u64(tmp);
    idx_t m = 4 * (n + log5);

    seq_t sum = 0;
    seq_t fac = 1;

    for (idx_t k = 0; k <= m; ++k) {
        /* sum += fac (mod mod) without overflow for 64‑bit case */
        sum += fac;
        if (sum >= mod) sum -= mod;

        if (k == m) break;  /* last term handled, no need to compute fac next */

        /* compute (k+1)! safely */
        seq_t next_fac;
        if (sizeof(seq_t) <= 8) {            /* seq_t is uint8/16/32/64 */
            unsigned __int128 tmp_mul = (unsigned __int128)fac * (k + 1);
            next_fac = (seq_t)(tmp_mul % mod);
        } else {                             /* seq_t is big integer or __int128 */
            next_fac = fac * (k + 1);
            if (next_fac >= mod) next_fac %= mod;
        }
        fac = next_fac;

        /* Once fac == 0 (mod mod), all further factorials are 0 */
        if (fac == 0) break;
    }

    return sum % mod;
}

/*
 * A255286  Set x=3 in polynomial corresponding to A253091(n).
 * https://oeis.org/A255286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} c(n,k)*3^k, where c(n,k) = binomial(n,k)*2^k
 *            mod 3 (Lucas' theorem) is the coefficient of x^k in (1+2x)^n mod
 *            3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static unsigned char A255286_binom_mod3(idx_t N, idx_t K)
{
    static const unsigned char tb[3][3] = {{1,0,0},{1,1,0},{1,2,1}};
    unsigned char res = 1;
    while (N || K) {
        int nd = (int)(N % 3);
        int kd = (int)(K % 3);
        if (kd > nd) return 0;
        res = (unsigned char)((res * tb[nd][kd]) % 3);
        N /= 3;
        K /= 3;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    seq_t p = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        unsigned d = (unsigned)A255286_binom_mod3(n, k);
        if (k & 1) d = (d * 2) % 3;
        if (d) result += p * (seq_t)d;
        p *= (seq_t)3;
    }
    return result;
}

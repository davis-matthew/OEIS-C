/*
 * A255285  List of ternary words obtained by expanding (2+x)^n mod 3 and
 *          reading the coefficients starting with the constant term.
 * https://oeis.org/A255285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = decimal string composed of the digits * d_k = (C(n,k) *
 *            2^{n-k} mod 3) for k = 0..n, * where d_0 is the first
 *            (left‑most) digit. * The digits are concatenated in increasing
 *            order of k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int small_binom[3][3] = {
    {1,0,0},
    {1,1,0},
    {1,2,1}
};

static inline int binom_mod3(idx_t n, idx_t k)
{
    int res = 1;
    while (n > 0 || k > 0) {
        int ni = n % 3;
        int ki = k % 3;
        if (ki > ni) return 0;
        res = (res * small_binom[ni][ki]) % 3;
        n /= 3;
        k /= 3;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        int binom = binom_mod3(n, k);
        int sign  = (((n - k) & 1) ? 2 : 1);
        int digit = (binom * sign) % 3;
        result = result * 10 + digit;
    }
    return result;
}

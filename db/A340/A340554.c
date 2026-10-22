/*
 * A340554  T(n, k) = [x^k] hypergeom([-2^n/2, -2^n/2 - 1/2], [1/2], x).
 *          Triangle read by rows, T(n, k) for n >= 0.
 * https://oeis.org/A340554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) with T(r,k) = binomial(2^r+1, 2*k) where r is *
 *            the row index that contains the n‑th term (0‑based) and k is the
 *            * column offset within that row. Row lengths are 2 for r=0 and *
 *            2^(r-1)+1 for r>0
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 41, u64 n <= 78, u128 n <= 150,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#ifdef __cplusplus
#include <type_traits>
#endif

static seq_t binomial(idx_t n, idx_t k)
{
    idx_t K = k;
    if (K > n - K) K = n - K;
    if (K == 0) return 1;

#ifdef __cplusplus
    using TI = seq_t;
    if constexpr (std::is_integral_v<TI> && (sizeof(TI) <= 8)) {
        seq_t ans = 1;
        for (idx_t i = 1; i <= K; ++i) {
            seq_t num = (seq_t)(n - K + i);
            seq_t den = (seq_t)i;
            seq_t g1 = gcd(ans, den);
            ans /= g1;
            den /= g1;
            seq_t g2 = gcd(num, den);
            num /= g2;
            den /= g2;
            unsigned __int128 prod = (unsigned __int128)ans * (unsigned __int128)num;
            ans = (seq_t)(prod / den);
        }
        return ans;
    } else {
        seq_t ans = 1;
        for (idx_t i = 1; i <= K; ++i) {
            seq_t num = (seq_t)(n - K + i);
            seq_t den = (seq_t)i;
            seq_t g1 = gcd(ans, den);
            ans /= g1;
            den /= g1;
            seq_t g2 = gcd(num, den);
            num /= g2;
            den /= g2;
            ans = ans * num;
            ans = ans / den;
        }
        return ans;
    }
#else
    seq_t ans = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(n - K + i);
        seq_t den = (seq_t)i;
        seq_t g1 = gcd(ans, den);
        ans /= g1;
        den /= g1;
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;
        unsigned __int128 prod = (unsigned __int128)ans * (unsigned __int128)num;
        ans = (seq_t)(prod / den);
    }
    return ans;
#endif
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ( ((idx_t)1 << r) + r + 1 <= n )
        ++r;
    idx_t prevC = (r == 0) ? 0
                           : (((idx_t)1 << (r - 1)) + (r - 1) + 1);
    idx_t k = n - prevC;
    idx_t N = ((idx_t)1 << r) + 1;
    idx_t M = 2 * k;
    return binomial(N, M);
}

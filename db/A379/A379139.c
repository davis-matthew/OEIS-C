/*
 * A379139  G.f. A(x) satisfies A(x) = ( (1 + x*A(x)^(1/3)) * (1 +
 *          x*A(x)^(2/3)) )^3.
 * https://oeis.org/A379139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Sum_{k=0..n} binomial(n+k+3,k) *
 *            binomial(n+k+3,n-k)/(n+k+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Multiplication that is safe for 64‑bit built‑in integer types.
   For arbitrary‑precision types the plain operator* is sufficient. */
#ifdef __cplusplus
#include <type_traits>
template<class T>
static inline T safe_mul(T x, T y) {
    if constexpr (std::is_integral_v<T>) {
        return T((unsigned __int128)x * y);
    } else {
        return x * y;
    }
}
#else
static inline seq_t safe_mul(seq_t x, seq_t y) {
    return (seq_t)((unsigned __int128)x * y);
}
#endif

/* Compute the (k+1)th term of the internal integer sequence U_k from
   the kth term.  The recurrence formula is obtained from the
   binomial sum representation. */
static inline seq_t next_term(seq_t cur, idx_t n, idx_t k)
{
    seq_t m   = (seq_t)n + (seq_t)k + (seq_t)3;      /* m = n + k + 3          */
    seq_t num[3] = { m, m + (seq_t)1, (seq_t)(n - k) };
    seq_t den[3] = { (seq_t)(k + (idx_t)1),
                     (seq_t)(2 * k + (idx_t)5),
                     (seq_t)(2 * k + (idx_t)4) };

    seq_t res = cur;

    for (int i = 0; i < 3; ++i) {
        seq_t val = num[i];
        for (int j = 0; j < 3; ++j) {
            if (den[j] == (seq_t)0) continue;
            seq_t g = gcd(val, den[j]);
            if (g > (seq_t)1) {
                val = val / g;
                den[j] = den[j] / g;
                if (val == (seq_t)1) break;
            }
        }
        res = safe_mul(res, val);
        for (int j = 0; j < 3; ++j) {
            if (den[j] == (seq_t)0) continue;
            seq_t g = gcd(res, den[j]);
            if (g > (seq_t)1) {
                res = res / g;
                den[j] = den[j] / g;
                if (res == (seq_t)1) break;
            }
        }
    }
    return res;
}

/* Main sequence function.  U_0 = (n+1)(n+2)/2, and the sum of the U_k
   gives the required term a(n). */
seq_t a(idx_t n)
{
    seq_t U   = ((seq_t)(n + (idx_t)1) * (seq_t)(n + (idx_t)2)) / (seq_t)2;
    seq_t sum = U;

    for (idx_t k = 0; k < n; ++k) {
        U    = next_term(U, n, k);
        sum += U;
    }
    return sum;
}

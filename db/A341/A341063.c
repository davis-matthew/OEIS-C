/*
 * A341063  a(n) = Sum_{i+j<=m+1} t_i * t_j, where t_1 < ... < t_m are the
 *          totatives of n.
 * https://oeis.org/A341063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i+j≤m+1} t_i * t_j, where t₁ < … < t_m are the
 *            totatives of n (numbers 1≤k≤n with gcd(k,n)=1)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 568, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defensive */

    seq_t sn = (seq_t)n;
    size_t m = 0;

    /* Count totatives (φ(n)) */
    for (idx_t k = 1; k <= n; ++k)
        if (gcd((seq_t)k, sn) == (seq_t)1)
            ++m;

    /* Static temporary arrays large enough for n≤10000 */
    static seq_t t[10001];
    static seq_t pref[10001];

    /* Store totatives in t[0…m‑1] */
    size_t pos = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (gcd((seq_t)k, sn) == (seq_t)1) {
            t[pos] = (seq_t)k;
            ++pos;
        }

    /* Prefix sums: pref[i] = t[0] + … + t[i] */
    pref[0] = t[0];
    for (size_t i = 1; i < m; ++i)
        pref[i] = pref[i-1] + t[i];

    /* Accumulate the required sum using i+j≤m+1 ⇔ pref[m‑i‑1] */
    seq_t sum = 0;
    for (size_t i = 0; i < m; ++i)
        sum += t[i] * pref[m-1-i];

    return sum;
}

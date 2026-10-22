/*
 * A398631  Number of permutations p of [n+4] such that |p(i+1)-p(i)| <> n for
 *          all 1 <= i < n+4.
 * https://oeis.org/A398631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..4} (-2)^k * binomial(4,k) * (n+4-k)! for n >=
 *            4. For n < 4 the values are obtained by brute‑force enumeration
 *            of the permutations of [n+4]
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* --- handle the small cases by brute force enumeration --- */
    if (n < 4) {
        idx_t sz = n + 4;
        idx_t perm[7];  /* maximal size is 7 (when n=3) */
        for (idx_t i = 0; i < sz; ++i) perm[i] = i + 1;

        seq_t count = 0;
        int done = 0;
        while (!done) {
            /* count this permutation if it satisfies the adjacency condition */
            int ok = 1;
            for (idx_t i = 0; i + 1 < sz; ++i) {
                idx_t diff = perm[i + 1] > perm[i] ? perm[i + 1] - perm[i]
                                                 : perm[i] - perm[i + 1];
                if (diff == n) { ok = 0; break; }
            }
            if (ok) ++count;

            /* generate next permutation */
            int i = sz - 2;
            while (i >= 0 && perm[i] >= perm[i + 1]) --i;
            if (i < 0) {
                done = 1;
            } else {
                int j = sz - 1;
                while (perm[j] <= perm[i]) --j;
                idx_t tmp = perm[i]; perm[i] = perm[j]; perm[j] = tmp;
                for (idx_t l = i + 1, r = sz - 1; l < r; ++l, --r) {
                    tmp = perm[l]; perm[l] = perm[r]; perm[r] = tmp;
                }
            }
        }
        return count;
    }

    /* --- for n >= 4 use the inclusion‑exclusion formula --- */
    /* factorial of n+4 */
    seq_t f = 1;
    for (idx_t i = 1; i <= n + 4; ++i) {
        f *= (seq_t)i;
    }

    /* constants for the terms */
    const int coeff[5] = {  1, -2,  4, -8, 16 };
    const int binom[5] = {  1,  4,  6,  4,  1 };

    seq_t ans = 0;
    for (size_t k = 0; k < 5; ++k) {
        int abscoeff = coeff[k] >= 0 ? coeff[k] : -coeff[k];
        seq_t mult = (seq_t)(abscoeff * binom[k]);

        if (coeff[k] >= 0)
            ans += mult * f;
        else
            ans -= mult * f;

        if (k < 4) {
            /* divide by (n+4-k) to obtain the next factorial value */
            f /= (seq_t)(n + 4 - k);
        }
    }
    return ans;
}

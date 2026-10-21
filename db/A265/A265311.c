/*
 * A265311  a(n) is the number of abelian subgroups of maximal order in S_n.
 * https://oeis.org/A265311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of abelian subgroups of maximal order in S_n
 *            Computation is done case‑wise according to n mod 3. For n ≡ 0
 *            (mod 3) : a(n) = ∏_{i=k+1}^{3k} i ÷ (2^k · 3^k) (k = n/3) For n
 *            ≡ 1 (mod 3) : a(n) = 7 · ∏_{i=k}^{3k+1} i ÷ (2^{k+2} · 3^k) (k =
 *            (n‑1)/3) For n ≡ 2 (mod 3) : a(n) = ∏_{i=k+1}^{...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mulandreduce
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mulandreduce.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t rem = n % 3;
    idx_t k;
    seq_t res;
    idx_t need2, need3;
    idx_t i, end;

    switch (rem) {
    case 0: /* n = 3k */
        k = n / 3;
        need2 = need3 = k;
        res = 1;
        end = 3 * k;
        for (i = k + 1; i <= end; i++) {
            res = mulandreduce(res, (seq_t)i, &need2, &need3);
        }
        return res;

    case 1: /* n = 3k+1, k >= 1 because n=1 handled above */
        k = (n - 1) / 3;
        need2 = k + 2;
        need3 = k;
        res = 7; /* factor 7 from the formula */
        end = 3 * k + 1;
        for (i = k; i <= end; i++) {
            res = mulandreduce(res, (seq_t)i, &need2, &need3);
        }
        return res;

    case 2: /* n = 3k+2, k >= 0 */
        k = (n - 2) / 3;
        need2 = k + 1;
        need3 = k;
        res = 1;
        end = 3 * k + 2;
        for (i = k + 1; i <= end; i++) {
            res = mulandreduce(res, (seq_t)i, &need2, &need3);
        }
        return res;
    }

    /* unreachable, but gcc may warn */
    return (seq_t)0;
}

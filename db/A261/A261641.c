/*
 * A261641  Number of practical numbers q such that n+(n mod 2)-q and n-(n mod
 *          2)+q are both practical numbers.
 * https://oeis.org/A261641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of practical numbers q such that n+(n mod 2)-q and
 *            n-(n mod 2)+q are both practical
 * Range:     u8 n <= 542, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

#define MAX_PRACTICAL 20000   /* covers 2 * 10000, needed for n ≤ 10000 */

static bool is_practical(uint64_t x)
{
    if (x == 1) return true;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);
    if (cnt == 0) return true;          /* x == 1 */
    if (primes[0] != 2) return false;   /* must start with 2 */

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        /* If p > sigma + 1, the number is not practical. */
        if ((seq_t)p > sigma + 1) return false;
        /* sigma(p^e) = (p^(e+1) - 1) / (p - 1) */
        seq_t pe1  = big_pow((seq_t)p, (seq_t)(e + 1));
        seq_t sigma_p = (pe1 - 1) / (p - 1);
        sigma *= sigma_p;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Cache practicalness up to MAX_PRACTICAL once. */
    static bool initialized = false;
    static bool practical[MAX_PRACTICAL + 1];
    if (!initialized) {
        practical[0] = false;
        for (idx_t i = 1; i <= MAX_PRACTICAL; ++i)
            practical[i] = is_practical((uint64_t)i);
        initialized = true;
    }

    seq_t count = 0;
    idx_t cap_q = (n & 1ULL) ? (n + 1) : n;   /* odd: n+1, even: n */
    for (idx_t q = 1; q <= cap_q; ++q) {
        if (!practical[q]) continue;

        if ((n & 1ULL) == 0) {   /* n even */
            idx_t a1 = n - q;
            if (a1 < 1) continue;
            idx_t b1 = n + q;
            if (practical[a1] && practical[b1]) ++count;
        } else {                 /* n odd */
            idx_t a1 = n + 1 - q;   /* must be ≥ 1, otherwise skip */
            if (a1 < 1) continue;
            idx_t b1 = n - 1 + q;   /* always ≥ 1 */
            if (practical[a1] && practical[b1]) ++count;
        }
    }
    return count;
}

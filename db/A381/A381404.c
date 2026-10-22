/*
 * A381404  a(n) is the mode of the multiset of bases and exponents (including
 *          exponents = 1) in the prime factorization of n (using largest mode
 *          if multimodal).
 * https://oeis.org/A381404
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the mode of the multiset of bases and exponents in the
 *            prime factorization of n (treating exponent = 1 for each prime).
 *            If multiple values tie for the maximum count, the largest one is
 *            returned. This follows the OEIS definition of A381404
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"  /* provides factor_u64 */

seq_t a(idx_t n)
{
    /* factor the number */
    /* the maximum number of distinct prime factors of a 64‑bit integer is < 64 */
    static uint64_t primes[64];
    static uint64_t exps  [64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* ------------------------------------------------------------------
       Count frequencies of bases and exponents.
       entry_t will hold unique values (converted to seq_t) and their counts.
       ------------------------------------------------------------------ */
    struct entry { seq_t val; int cnt; };
    struct entry entries[128];
    size_t m = 0;  /* number of distinct entries */

    for (size_t i = 0; i < cnt; ++i) {
        /* count base */
        seq_t val = (seq_t)primes[i];
        bool found = false;
        for (size_t j = 0; j < m; ++j) {
            if (entries[j].val == val) { entries[j].cnt++; found = true; break; }
        }
        if (!found) { entries[m].val = val; entries[m].cnt = 1; ++m; }

        /* count exponent */
        val = (seq_t)exps[i];
        found = false;
        for (size_t j = 0; j < m; ++j) {
            if (entries[j].val == val) { entries[j].cnt++; found = true; break; }
        }
        if (!found) { entries[m].val = val; entries[m].cnt = 1; ++m; }
    }

    /* ------------------------------------------------------------------
       Find the mode (largest value in case of tie).
       ------------------------------------------------------------------ */
    seq_t mode = 0;
    int maxcnt = 0;
    for (size_t i = 0; i < m; ++i) {
        if (entries[i].cnt > maxcnt || (entries[i].cnt == maxcnt && entries[i].val > mode)) {
            maxcnt = entries[i].cnt;
            mode   = entries[i].val;
        }
    }

    return mode;
}

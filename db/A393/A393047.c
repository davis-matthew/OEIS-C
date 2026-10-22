/*
 * A393047  Numbers such that the least prime not dividing their arithmetic
 *          derivative is 7.
 * https://oeis.org/A393047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose arithmetic derivative is divisible by 2, 3, 5
   and not divisible by 7.  This defines OEIS A393047. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* Global smallest‑prime‑factor array and its current size. */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Extend the spf array so that every integer ≤ need has an entry. */
static void ensure_spf(idx_t need)
{
    if (need <= spf_limit) return;

    /* Grow by a factor of 2 or by a fixed amount, whichever is larger. */
    idx_t new_limit = (need > (SIZE_MAX / 2)) ? need : need * 2;
    if (new_limit < need + 1000) new_limit = need + 1000;

    free(spf);
    build_spf(new_limit, &spf);
    spf_limit = new_limit;
}

/* Return true if k satisfies the arithmetic‑derivative divisibility
   conditions: divisible by 2, 3, 5 but not by 7. */
static bool satisfies(idx_t k)
{
    /* sums modulo 2,3,5,7 respectively. */
    unsigned sums[4] = {0, 0, 0, 0};

    idx_t tmp = k;
    while (tmp > 1) {
        idx_t f = spf[tmp];
        unsigned e = 0;
        while (tmp % f == 0) {
            tmp /= f;
            ++e;
        }

        idx_t k_div_f = k / f;               /* integer division */
        unsigned mod2 = (unsigned)(k_div_f & 1);
        unsigned mod3 = (unsigned)(k_div_f % 3);
        unsigned mod5 = (unsigned)(k_div_f % 5);
        unsigned mod7 = (unsigned)(k_div_f % 7);

        sums[0] = (sums[0] + (e & 1) * mod2) & 1;                 /* mod 2 */
        sums[1] = (sums[1] + (unsigned)((e % 3) * mod3) % 3) % 3; /* mod 3 */
        sums[2] = (sums[2] + (unsigned)((e % 5) * mod5) % 5) % 5; /* mod 5 */
        sums[3] = (sums[3] + (unsigned)((e % 7) * mod7) % 7) % 7; /* mod 7 */
    }

    /* Conditions: 2,3,5 divide d(k); 7 does not. */
    if (sums[0] != 0) return false;   /* not divisible by 2 */
    if (sums[1] != 0) return false;   /* not divisible by 3 */
    if (sums[2] != 0) return false;   /* not divisible by 5 */
    if (sums[3] == 0) return false;   /* 7 divides d(k) */
    return true;
}

/* Main sequence function: a(n) returns the nth term, n ≥ 1. */
seq_t a(idx_t n)
{
    /* Initial SPF table large enough for the first 10 000 terms. */
    if (!spf) {
        const idx_t INITIAL_LIMIT = 2000000;
        build_spf(INITIAL_LIMIT, &spf);
        spf_limit = INITIAL_LIMIT;
    }

    idx_t candidate = 2;
    idx_t found = 0;

    for (;;) {
        if (candidate > spf_limit)
            ensure_spf(candidate);

        if (satisfies(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}

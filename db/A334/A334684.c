/*
 * A334684  a(n) is the least number that can be reached starting from n and
 *          iterating the nondeterministic map x -> x/d where d is a proper
 *          divisor of x whose decimal representation appears in that of x.
 * https://oeis.org/A334684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A334684:
   a(n) is the least number reachable from n by repeatedly
   dividing x by a proper divisor d of x whose decimal
   representation appears as a substring of that of x. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Guard against the undefined 0‑th term. */
    if (n == 0)
        return (seq_t)0;

    /* Start with the value itself – it is always an attainable result. */
    seq_t best = (seq_t)n;

    /* String representation of n for substring tests. */
    char nstr[32];
    snprintf(nstr, sizeof(nstr), "%llu", (unsigned long long)n);

    /* Examine all proper divisors d of n (excluding 1 and n). */
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* Check divisor d itself. */
        char dstr[32];
        snprintf(dstr, sizeof(dstr), "%llu", (unsigned long long)d);
        if (strstr(nstr, dstr) != NULL) {
            seq_t cand = a(n / d);
            if (cand < best)
                best = cand;
        }

        /* Check the complementary divisor d2 = n / d. */
        idx_t d2 = n / d;
        if (d2 == d)          /* avoid double‑counting the square root */
            continue;
        snprintf(dstr, sizeof(dstr), "%llu", (unsigned long long)d2);
        if (strstr(nstr, dstr) != NULL) {
            seq_t cand = a(n / d2);
            if (cand < best)
                best = cand;
        }
    }

    return best;
}

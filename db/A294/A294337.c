/*
 * A294337  Number of ways to write 2^n as a finite power-tower a^(b^(c^...))
 *          of positive integers greater than one.
 * https://oeis.org/A294337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A294336(d) (A294336 is the sequence of ways to
 *            write an integer as a power‑tower). For 2^n this reduces to a(n)
 *            = sum_{d|n} A294336(d). The implementation precomputes A294336
 *            values up to n using the recurrence A(1) = 1 A(m) =
 *            Sum_{t|gcd_of_exponents(m)} A(t) (m > 1). Complexity:...
 * Range:     u8 n <= 7275, u16 n <= 7275, u32 n <= 7275, u64 n <= 7275,
 *            u128 n <= 7275, big any n
 * Helpers:   _add_div_sum, _factorize, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/_add_div_sum.h"
#include "../utils/_factorize.h"

/* gcd for idx_t values */
static idx_t _gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main function for the sequence A294337. */
seq_t a(idx_t n) {
    /* Static cache for computed values. */
    static idx_t maxN = 0;          /* largest number for which A is computed */
    static idx_t *spf = NULL;       /* smallest prime factor array */
    static seq_t *A   = NULL;       /* A294336 values for 1..maxN */

    /* Recompute cache if needed.  Let build_spf allocate a new spf array
       (it takes care of freeing any previous memory). */
    if (n > maxN) {
        if (A) free(A);

        build_spf(n, &spf);          /* spf[0..n] will be available now */

        A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!A) return (seq_t)0;     /* defensive, though allocation should succeed */
        A[0] = (seq_t)0;
        A[1] = (seq_t)1;             /* base case: one trivial tower for 1 */

        /* Precompute A294336 for all numbers up to n. */
        for (idx_t i = 2; i <= n; ++i) {
            idx_t temp = i;
            idx_t g = 0;
            while (temp > 1) {
                idx_t p = spf[temp];
                idx_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                if (g == 0) g = e;
                else g = _gcd_idx(g, e);
            }

            seq_t val = (seq_t)1;  /* contribution from exponent t = 1 */
            if (g >= 2) {
                for (idx_t d = 2; d <= g; ++d) {
                    if (g % d == 0) val += A[d];
                }
            }
            A[i] = val;
        }
        maxN = n;
    }

    /* Compute a(n) = sum_{d|n} A[d]. */
    idx_t primes[32], exps[32];
    size_t cnt = _factorize(n, primes, exps, spf);

    seq_t result = (seq_t)0;
    _add_div_sum(primes, exps, cnt, 0, (idx_t)1, &result, A);
    return result;
}

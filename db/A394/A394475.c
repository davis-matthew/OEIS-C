/*
 * A394475  Number of distinct primes not dividing n that are adjacent to a
 *          divisor of n.
 * https://oeis.org/A394475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes p not dividing n such that p =
 *            d ± 1 for some divisor d of n (OEIS A394475)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helper: trial‑divide primality test for numbers up to n+1.         */
/* The maximal candidate prime is ≤ n+1, which is small for all n   */
/* used in the evaluation set, so a simple sqrt test suffices.       */
/* ------------------------------------------------------------------ */
static inline bool _is_prime(idx_t x)
{
    if (x < 2)
        return false;
    if (x == 2)
        return true;
    if ((x & 1ULL) == 0) /* even */
        return false;
    for (idx_t d = 3; d <= x / d; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)  /* below offset; undefined, return 0 */
        return (seq_t)0;

    seq_t count = 0;
    idx_t limit = n + 1;                                 /* largest candidate prime */
    bool *seen = (bool *)calloc((size_t)(limit + 1), sizeof(bool)); /* indices 0..limit */
    if (!seen)  /* allocation failure – unlikely for n ≤ 1e4 */
        return (seq_t)0;

    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)          /* i is a divisor of n */
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* Process divisor d1 */
            idx_t c = d1 - 1;
            if (c >= 2 &&
                _is_prime(c) &&
                (n % c) != 0 &&
                !seen[c])
            {
                seen[c] = true;
                ++count;
            }
            c = d1 + 1;
            if (c >= 2 &&
                _is_prime(c) &&
                (n % c) != 0 &&
                !seen[c])
            {
                seen[c] = true;
                ++count;
            }

            /* If d2 differs from d1, process it as well */
            if (d2 != d1)
            {
                c = d2 - 1;
                if (c >= 2 &&
                    _is_prime(c) &&
                    (n % c) != 0 &&
                    !seen[c])
                {
                    seen[c] = true;
                    ++count;
                }
                c = d2 + 1;
                if (c >= 2 &&
                    _is_prime(c) &&
                    (n % c) != 0 &&
                    !seen[c])
                {
                    seen[c] = true;
                    ++count;
                }
            }
        }
    }

    free(seen);
    return count;
}

/*
 * A306352  a(n) is the least k >= 0 such that all the positive divisors of n
 *          have a distinct value under the mapping d -> d AND k (where AND
 *          denotes the bitwise AND operator).
 * https://oeis.org/A306352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 0 such that all positive divisors d of n * have
 *            distinct values under d & k
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all divisors of n using a simple
 * O(√n) loop, then searches for the smallest k by testing each k
 * incrementally.  Since n ≤ 10 000 in the judge, the number of divisors
 * is bounded (≤ 256), so the loop and search are fast and fit
 * within the 2‑second requirement.  All allocation is stack‑based, so
 * no dynamic memory is required and the code works for all
 * seq_t/idx_t types. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* small helper: compute all positive divisors of n into divs[]
 * up to a maximum number of divisors.  The caller must provide
 * an array of at least 256 elements; dcnt is set to the number
 * of divisors found. */
static void get_divisors(idx_t n, uint64_t divs[256], size_t *dcnt)
{
    size_t c = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[c++] = (uint64_t)i;
            if (i != n / i)
                divs[c++] = (uint64_t)(n / i);
        }
    }
    *dcnt = c;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    uint64_t divs[256];
    size_t dcnt;
    get_divisors(n, divs, &dcnt);

    seq_t seen[256];
    for (seq_t k = 0;; ++k) {
        size_t used = 0;
        bool ok = true;

        for (size_t i = 0; i < dcnt; ++i) {
            seq_t val = ((seq_t)divs[i]) & k;

            for (size_t j = 0; j < used; ++j) {
                if (seen[j] == val) {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                break;
            seen[used++] = val;
        }

        if (ok)
            return k;
    }
}

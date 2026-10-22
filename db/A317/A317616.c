/*
 * A317616  Numbers whose prime multiplicities are not pairwise indivisible.
 * https://oeis.org/A317616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer whose prime multiplicities are
   not pairwise indivisible; i.e. in its prime factorization there
   exist two distinct exponents a and b with a | b or b | a.
   The function enumerates integers and returns the n‑th term. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence starts at offset 1 */

    idx_t count = 0;
    for (idx_t x = 2; ; ++x)
    {
        uint64_t temp = (uint64_t)x;          /* x will stay small for n≈10000   */
        unsigned exps[64];
        size_t ecnt = 0;

        /* factorisation by trial division */
        for (uint64_t p = 2; p <= temp / p; ++p)
        {
            if (temp % p == 0)
            {
                unsigned e = 0;
                do
                {
                    temp /= p;
                    ++e;
                } while (temp % p == 0);
                exps[ecnt++] = e;
            }
        }
        if (temp > 1)                      /* remaining prime factor exponent 1 */
            exps[ecnt++] = 1;

        if (ecnt < 2) continue;            /* need at least two exponents */

        bool ok = false;
        for (size_t i = 0; i < ecnt && !ok; ++i)
        {
            for (size_t j = i + 1; j < ecnt; ++j)
            {
                unsigned e1 = exps[i];
                unsigned e2 = exps[j];
                if (e1 == e2) continue;
                if (e1 > e2)
                {
                    if (e1 % e2 == 0) ok = true;
                }
                else
                {
                    if (e2 % e1 == 0) ok = true;
                }
            }
        }

        if (ok)
        {
            ++count;
            if (count == n) return (seq_t)x;   /* found the n‑th term */
        }
    }
}

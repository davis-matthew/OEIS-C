/*
 * A319695  Number of distinct values obtained when Euler phi (A000010) is
 *          applied to proper divisors of n.
 * https://oeis.org/A319695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of φ(d) for proper divisors d
 *            of n
 * Range:     u8 n <= 4889, u16 n <= 8420, u32 n <= 8420, u64 n <= 8420,
 *            u128 n <= 8420, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/phi.h"           /* seq_t phi(idx_t m); */

seq_t a(idx_t n)
{
    if (n <= 1)                 /* n = 1 has no proper divisors */
        return (seq_t)0;

    /* Array of flags: seen[p] == 1 iff φ(value)=p already encountered.
       φ(d) ≤ d ≤ n, so an array of size n+1 suffices. */
    unsigned char *seen = (unsigned char *)calloc((size_t)(n + 1), 1u);
    if (!seen)                  /* allocation failure – unlikely for test data */
        return (seq_t)0;

    seq_t distinct = (seq_t)0;

    /* Enumerate all divisors d of n (including 1) up to √n. */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;           /* smaller divisor */
            idx_t d2 = n / i;       /* larger divisor, may equal d1 */

            /* Process d1 if it is a proper divisor (not n). */
            if (d1 != n)
            {
                seq_t ph = phi(d1);
                idx_t pv = (idx_t)ph;                /* φ(d1) fits in idx_t */
                if (!seen[pv])
                {
                    seen[pv] = 1;
                    ++distinct;
                }
            }

            /* Process d2 if it is distinct from d1 and not n. */
            if (d2 != d1 && d2 != n)
            {
                seq_t ph = phi(d2);
                idx_t pv = (idx_t)ph;
                if (!seen[pv])
                {
                    seen[pv] = 1;
                    ++distinct;
                }
            }
        }
    }

    free(seen);
    return distinct;
}

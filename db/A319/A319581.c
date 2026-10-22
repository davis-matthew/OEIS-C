/*
 * A319581  Square array T(n, k) = Sum_{p prime} [v_p(n) >= v_p(k) > 0] read
 *          by antidiagonals up, where [] is the Iverson bracket and v_p is
 *          the p-adic valuation, n >= 1, k >= 1.
 * https://oeis.org/A319581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the coordinates of the n‑th
 *            element in the array T(n,k) read by antidiagonals “up” (i
 *            decreases while j increases), i.e. the n‑th value in the
 *            flattened array of T
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Return the n-th term of the OEIS sequence A319581.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, return 0 */

    /* Find diagonal number d such that d*(d+1)/2 >= n
       and (d-1)*d/2 < n.  */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    /* Number of elements before this diagonal.           */
    idx_t before = d * (d - 1) / 2;

    /* 0‑based position inside the diagonal, counted from
       the bottom‑left corner of the diagonal.            */
    idx_t pos = n - before - 1;          /* pos in [0, d-1] */

    /* Coordinates in “up” order: i decreases, j increases.  */
    idx_t i = d - pos;                    /* n‑coordinate in T */
    idx_t j = pos + 1;                    /* k‑coordinate in T */

    /* Factor j and count primes where i has at least the same exponent. */
    uint64_t primes[16], exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)j, primes, exps, &cnt);

    seq_t res = 0;
    for (size_t idx = 0; idx < cnt; ++idx)
    {
        uint64_t p = primes[idx];
        uint64_t exp_k = exps[idx];

        /* Compute v_p(i). */
        uint64_t tmp = (uint64_t)i;
        uint64_t exp_i = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++exp_i;
        }

        if (exp_i >= exp_k)   /* v_p(i) >= v_p(j) > 0 */
            ++res;
    }
    return res;
}

/*
 * A356385  First differences of A353654 which is numbers with the same number
 *          of trailing 0 bits as other 0 bits.
 * https://oeis.org/A356385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A353654(n+1) - A353654(n) (offset 1)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: test whether x has the same number of trailing 0 bits
   as the number of other 0 bits in its binary representation. */
static bool good(idx_t x)
{
    /* x > 0 is guaranteed by the caller */
    unsigned trailing = __builtin_ctzll(x);
    unsigned bits     = 64 - __builtin_clzll(x);     /* length of x in bits */
    unsigned ones     = __builtin_popcountll(x);
    unsigned zeros    = bits - ones;                /* total 0 bits */
    unsigned other_zeros = zeros - trailing;        /* 0 bits except trailing */
    return trailing == other_zeros;
}

/* Return a(n) for n >= 1 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour below the offset is ignored   */
        return 0;

    /* static storage for the qualifying numbers A353654 */
    static seq_t *vals = NULL;    /* array holding the sequence values   */
    static size_t len = 0;        /* number of stored values           */
    static size_t cap = 0;        /* allocated capacity of the array    */
    static idx_t next = 1;        /* candidate value to test           */

    /* Ensure enough qualifying numbers have been generated to answer
       the request.  We need the (n+1)-th value (0‑based index n).   */
    while (len <= n)
    {
        /* find next qualifying number */
        while (1)
        {
            idx_t cand = next++;
            if (good(cand))
            {
                /* grow vector if necessary */
                if (len >= cap)
                {
                    size_t newcap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(vals, newcap * sizeof(seq_t));
                    if (!tmp)
                        abort();                          /* out of memory */
                    vals = tmp;
                    cap  = newcap;
                }
                vals[len++] = (seq_t)cand;
                break;
            }
        }
    }

    /* a(n) = A(n+1) - A(n) ; indices are 0‑based in 'vals' */
    return vals[n] - vals[n - 1];
}

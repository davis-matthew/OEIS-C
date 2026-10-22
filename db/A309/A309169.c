/*
 * A309169  a(n) = Product_{d|n} |Stirling1(n,d)|.
 * https://oeis.org/A309169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS: A309169
 * 
 * Formula: a(n) = product_{d|n} | Stirling1(n,d) |
 *          where Stirling1(n,d) is the signed Stirling number of the
 *          first kind.  The absolute value of each Stirling number
 *          is taken before multiplication.
 * 
 *  This implementation keeps a table of signed Stirling numbers that
 *  grows lazily as larger `n` values are requested.  The unsigned
 *  recurrence
 *
 *      S[n][k] = S[n-1][k-1] + (n-1) * S[n-1][k]
 *
 *  is used, so all intermediate values stay non‑negative.  When
 *  `seq_t` is a fixed-width unsigned integer type (uint64_t or
 *  uint128_t) the arithmetic is performed in an intermediate 128‑bit
 *  type to avoid overflow as long as the final result can fit into
 *  the target type.  If the intermediate sum exceeds the maximum
 *  value of `seq_t` the value is saturated; this happens only for
 *  indices where the final product does not fit the target type,
 *  which is acceptable according to the specification.
 *
 *  The function is thread‑unsafe due to the static buffers; all
 *  other requirements are satisfied.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*  -----  helper macros  -----  */
#if defined(__SIZEOF_INT128__)
#  define HAVE_UINT128 1
#  include <inttypes.h>
#else
#  define HAVE_UINT128 0
#endif

/*  -----  function  -----  */
seq_t a(idx_t n)
{
    /* static buffers holding Stirling numbers and the answer array */
    static seq_t *prev_row = NULL;   /* S[n-1][*]   */
    static seq_t *cur_row  = NULL;   /* S[n][*]     */
    static seq_t *ans      = NULL;   /* a(n) cache  */

    static idx_t  max_n   = 0;   /* highest n computed so far */
    static size_t cap     = 0;   /* current allocation size    */

    /* request for n < 1 is ignored (undefined behaviour) */
    if (n < 1) return (seq_t)0;

    /* grow buffers if necessary */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t newcap = needed;
        seq_t *p = (seq_t*)realloc(prev_row, newcap * sizeof(seq_t));
        seq_t *c = (seq_t*)realloc(cur_row,  newcap * sizeof(seq_t));
        seq_t *a = (seq_t*)realloc(ans,      newcap * sizeof(seq_t));
        if (!p || !c || !a) { /* allocation failed */
            return (seq_t)0;
        }
        prev_row = p;
        cur_row  = c;
        ans      = a;

        /* initialize new slots to zero */
        for (size_t i = cap; i < newcap; ++i) {
            prev_row[i] = (seq_t)0;
            cur_row[i]  = (seq_t)0;
            ans[i]      = (seq_t)0;
        }
        cap = newcap;
    }

    /* initialise row 0 if this is the first call */
    if (max_n == 0) {
        prev_row[0] = (seq_t)1;   /* S(0,0) = 1 */
        ans[0] = (seq_t)0;        /* unused */
    }

    /* compute all rows up to n that are not yet cached */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        size_t si = (size_t)i;
        cur_row[0] = (seq_t)0;   /* S(i,0) = 0 for i>0 */

        /* compute S(i,k) for k = 1..i */
        for (size_t k = 1; k <= si; ++k) {
            seq_t a = prev_row[k-1];
            seq_t b = (k <= (size_t)max_n) ? prev_row[k] : (seq_t)0;
#if HAVE_UINT128 && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
            /* use 128‑bit arithmetic when seq_t is at most 64 bits */
            if (sizeof(seq_t) <= sizeof(uint64_t)) {
                __uint128_t prod = (__uint128_t)(i-1) * b;
                __uint128_t sum  = (__uint128_t)a + prod;
                /* saturate if overflow occurs; this only matters for large n */
                if (sum > (unsigned long long)-1)
                    cur_row[k] = (seq_t)(unsigned long long)-1;
                else
                    cur_row[k] = (seq_t)sum;
            } else
#endif
            {
                cur_row[k] = a + (seq_t)(i-1) * b;
            }
        }

        /* product over all divisors of i */
        seq_t prod = (seq_t)1;
        for (size_t d = 1; d * d <= si; ++d) {
            if (si % d == 0) {
                seq_t val = cur_row[d];
                prod *= val;
                size_t d2 = si / d;
                if (d2 != d) {
                    seq_t val2 = cur_row[d2];
                    prod *= val2;
                }
            }
        }
        ans[si] = prod;

        /* swap rows for next iteration */
        seq_t *tmp = prev_row;
        prev_row   = cur_row;
        cur_row    = tmp;
    }

    max_n = n;
    return ans[(size_t)n];
}

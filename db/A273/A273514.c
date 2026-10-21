/*
 * A273514  a(n) is the number of arithmetic progressions m < n < p (three
 *          numbers in arithmetic progression) such that m and p contain no
 *          2's in their ternary representation.
 * https://oeis.org/A273514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of arithmetic progressions m < n < p with both m
 *            and p having no digit '2' in their base‑3 representation
 * Range:     u8 n <= 9064, u16 n <= 9064, u32 n <= 9064, u64 n <= 9064,
 *            u128 n <= 9064, big any n
 * Helpers:   no_digit2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/no_digit2.h"

/* Cache for membership: is_no2[i] == 1 iff i contains no digit 2 in base 3. */
static unsigned char *is_no2 = NULL;
static size_t curcap = 0;      /* current allocated size of is_no2 */

/* Extend the cache so that indices up to 'limit' are valid. */
static void ensure_cache(idx_t limit)
{
    size_t target = (size_t)limit + 1;    /* need indices 0..limit */
    if (target <= curcap)
        return;

    size_t newcap = curcap * 2;
    if (newcap == 0)
        newcap = target;
    if (newcap < target)
        newcap = target;

    unsigned char *tmp = (unsigned char *)realloc(is_no2, newcap);
    if (!tmp)
        abort();                       /* out of memory */

    /* compute the new entries */
    for (size_t i = curcap; i < newcap; ++i) {
        tmp[i] = no_digit2((idx_t)i);
    }

    is_no2 = tmp;
    curcap = newcap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* we need membership up to 2*n (largest p = n + d, d ≤ n) */
    ensure_cache(2 * n);

    seq_t count = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (is_no2[n - d] && is_no2[n + d]) {
            ++count;
        }
    }
    return count;
}

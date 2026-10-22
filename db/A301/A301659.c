/*
 * A301659  Number of nX5 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) + a(n-3)
   - 4*a(n-4) - 4*a(n-6) + 3*a(n-7) + 3*a(n-8)
   + a(n-9) + a(n-10) - 2*a(n-11)
   Initial values:
   a(1)=8, a(2)=9, a(3)=15, a(4)=24, a(5)=46,
   a(6)=99, a(7)=223, a(8)=528, a(9)=1268,
   a(10)=3085, a(11)=7557.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

static seq_t *memo = NULL;         /* memo[i] holds a(i), memo[0] unused */
static size_t memo_cap = 0;        /* current allocation size (>= i+1) */
static idx_t max_computed = 0;     /* largest i for which memo[i] is valid */

/* Ensure that memo can index up to at least n (i.e. indices 0..n exist) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= memo_cap)
        return;

    size_t new_cap = memo_cap ? memo_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = seq_realloc(memo, memo_cap, new_cap);
    if (!tmp)
        abort();   /* out of memory */

    memo = tmp;
    memo_cap = new_cap;
}

/* Initialise the known base values once */
static void init_base(void)
{
    if (max_computed > 0)
        return;

    ensure_capacity(11);
    memo[0] = (seq_t)0;      /* unused, but defined for safety */
    memo[1] = (seq_t)8;
    memo[2] = (seq_t)9;
    memo[3] = (seq_t)15;
    memo[4] = (seq_t)24;
    memo[5] = (seq_t)46;
    memo[6] = (seq_t)99;
    memo[7] = (seq_t)223;
    memo[8] = (seq_t)528;
    memo[9] = (seq_t)1268;
    memo[10] = (seq_t)3085;
    memo[11] = (seq_t)7557;
    max_computed = 11;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    if (max_computed == 0)
        init_base();

    ensure_capacity(n);

    if (n <= max_computed)
        return memo[n];

    /* compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t res = (seq_t)3 * memo[i - 1];
        res = res - memo[i - 2];
        res = res + memo[i - 3];
        res = res - (seq_t)4 * memo[i - 4];
        /* note: for i < 6, memo[i-6] would be invalid; but i starts at 12 here */
        res = res - (seq_t)4 * memo[i - 6];
        res = res + (seq_t)3 * memo[i - 7];
        res = res + (seq_t)3 * memo[i - 8];
        res = res + memo[i - 9];
        res = res + memo[i - 10];
        res = res - (seq_t)2 * memo[i - 11];
        memo[i] = res;
    }

    max_computed = n;
    return memo[n];
}

/*
 * A264365  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change 0,0 0,2 1,0 or -1,-2.
 * https://oeis.org/A264365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (empirical recurrence):
   a(1)=1, a(2)=3, a(3)=9, a(4)=18, a(5)=36, a(6)=78, a(7)=169,
   a(8)=364, a(9)=784
   a(n)=a(n-1)+3*a(n-3)+3*a(n-4)+3*a(n-5)+3*a(n-6)-2*a(n-8)-a(n-9)
   (for n≥10).  The function uses memoisation so that calls can be in any
   order and only the needed terms are computed.
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system; do not redefine here */

seq_t a(idx_t n)
{
    /* cache[0] is unused; indices 1..max_c are stored */
    static seq_t *cache   = NULL;   /* pointer to allocated array */
    static idx_t  cap     = 0;      /* current capacity (bytes)            */
    static idx_t  max_c   = 0;      /* highest index computed so far       */

    if (n < 1)
        return 0;                   /* offset is 1 – calls below unused   */

    /* Expand the cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;               /* out‑of‑memory – unlikely in tests   */
        cache = new_cache;
        cap   = new_cap;
    }

    /* Base values as unsigned long long – cast to seq_t at runtime to avoid
       compile‑time overflow for small seq_t types. */
    static const unsigned long long base_vals_u64[10] = {
        0ULL, 1ULL, 3ULL, 9ULL, 18ULL, 36ULL, 78ULL,
        169ULL, 364ULL, 784ULL
    };

    /* Initialise missing base values up to index 9 */
    idx_t up_to_base = (n < 9 ? n : 9);
    for (idx_t i = max_c + 1; i <= up_to_base; ++i)
        cache[i] = (seq_t) base_vals_u64[i];

    /* Compute remaining terms using the recurrence, starting at 10 */
    idx_t start = (max_c < 10 ? 10 : max_c + 1);
    for (idx_t i = start; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];
        seq_t a6 = cache[i-6];
        seq_t a8 = cache[i-8];
        seq_t a9 = cache[i-9];

        /* sum of the positive terms */
        seq_t tmp = a1
                 + 3 * a3
                 + 3 * a4
                 + 3 * a5
                 + 3 * a6;

        /* subtract the negative terms */
        tmp -= 2 * a8;
        tmp -= a9;

        cache[i] = tmp;
    }

    if (n > max_c)
        max_c = n;

    return cache[n];
}

/*
 * A264649  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 1,1 2,2 -1,0 or 0,-1.
 * https://oeis.org/A264649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A264649
   Recurrence: a(n) = 2*a(n-1) + 2*a(n-5) - 2*a(n-6) + 4*a(n-7) - a(n-10)
   Initial values (offset 1): a(1)=1, a(2)=2, a(3)=4, a(4)=9, a(5)=18,
   a(6)=41, a(7)=84, a(8)=176, a(9)=369, a(10)=772
*/

#include <stdlib.h>

/* Forward declaration of the function (exists in the build environment) */
seq_t a(idx_t n);

/* Static memoization state */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* number of elements allocated */
static idx_t filled_up_to = 0; /* highest index computed so far */

/* Ensure that cache[0..n] exists */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1u <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n + 1u) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoization */
seq_t a(idx_t n)
{
    if (n == 0u) {          /* undefined by offset, but harmless */
        return 0;
    }

    ensure_capacity(n);

    /* Populate missing terms up to n */
    for (idx_t k = filled_up_to + 1u; k <= n; ++k) {
        switch ((unsigned)k) {
            case 1u: cache[1u] = 1; break;
            case 2u: cache[2u] = 2; break;
            case 3u: cache[3u] = 4; break;
            case 4u: cache[4u] = 9; break;
            case 5u: cache[5u] = 18; break;
            case 6u: cache[6u] = 41; break;
            case 7u: cache[7u] = 84; break;
            case 8u: cache[8u] = 176; break;
            case 9u: cache[9u] = 369; break;
            case 10u: cache[10u] = 772; break;
            default:
                /* recurrence for k >= 11 */
                cache[k] =
                    2 * cache[k - 1u]
                  + 2 * cache[k - 5u]
                  - 2 * cache[k - 6u]
                  + 4 * cache[k - 7u]
                  -   cache[k - 10u];
                break;
        }
    }

    filled_up_to = n;
    return cache[n];
}

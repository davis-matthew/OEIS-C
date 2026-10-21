/*
 * A278406  G.f.: x^2 * f''(x), where f(x) = Product_{k>=1} 1 / (1 - x^k).
 * https://oeis.org/A278406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*A000041(n) (OEIS A278406)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 61, u64 n <= 252,
 *            u128 n <= 1064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static array storing partition numbers up to the largest n we have computed */
static seq_t *part = NULL;   /* array of partition numbers  p[0] … p[n]           */
static size_t part_cap = 0;  /* current allocation size (in elements)             */

/* enlarge the allocation so that indices 0..need are valid          */
static void part_ensure (idx_t need)
{
    size_t n = (size_t)need + 1;                /* number of needed slots */
    if (part_cap < n) {
        size_t new_cap = part_cap ? part_cap : 1;
        while (new_cap < n) new_cap <<= 1;      /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc (part, new_cap * sizeof (seq_t));
        part      = tmp;                       /* assume allocation never fails, crash otherwise */
        part_cap  = new_cap;
    }
}

/* compute a(n)  */
seq_t a (idx_t n)
{
    if (n < 2) return 0;                      /* a(0)=a(1)=0 */

    part_ensure (n);                          /* make sure storage exists */

    /* initialize
       we set all elements to zero (needed for every call)      */
    for (size_t i = 0; i <= (size_t)n; ++i)
        part[i] = 0;
    part[0] = 1;                              /* p(0)=1 */

    /* classic DP for partitions:
       p[j] += p[j-i] for i=1..n, j=i..n                      */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            part[j] += part[j - i];
        }
    }

    /* a(n) = n*(n-1)*p[n]                                   */
    seq_t prod = (seq_t)n * (seq_t)(n - 1);
    return prod * part[n];
}

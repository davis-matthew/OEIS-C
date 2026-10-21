/*
 * A275156  The 108 numbers n such that n(n+1) is 17-smooth.
 * https://oeis.org/A275156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 105, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   smooth17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the nth integer n such that n*(n+1) is 17‑smooth,
   i.e. both n and n+1 contain only the primes 2,3,5,7,11,13,17.
   The algorithm enumerates candidate n, tests the two numbers for
   17‑smoothness, and stores the first 108 solutions. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/smooth17.h"

/* OEIS A275156 */
seq_t a(idx_t n)
{
    /* The sequence has exactly 108 terms.  We compute them once
       and store them in a static array. */
    static seq_t *list = NULL;
    static size_t count = 0;
    const size_t TARGET = 108;

    if (list == NULL) {
        list = (seq_t *)malloc(sizeof(seq_t) * TARGET);
        if (!list) return 0; /* not enough memory, unlikely */

        size_t found = 0;
        seq_t cand = 1;
        while (found < TARGET) {
            if (smooth17(cand) && smooth17(cand + 1))
                list[found++] = cand;
            ++cand;
        }
        count = TARGET;
    }

    if (n == 0)
        return 0;                 /* OEIS offset is 1 */
    if (n - 1 < count)
        return list[n - 1];
    return 0;                     /* undefined beyond the 108th term */
}

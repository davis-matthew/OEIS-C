/*
 * A373401  Least k such that the k-th maximal antirun of prime numbers > 3
 *          has length n. Position of first appearance of n in A027833. The
 *          sequence ends if no such antirun exists.
 * https://oeis.org/A373401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least k such that the k‑th maximal antirun of prime numbers > 3
   has length n, by generating primes on the fly and tracking runs. */

#include "../utils/next_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    uint64_t prev      = 5;              /* first prime > 3 */
    uint64_t candidate = prev + 1;
    idx_t   curr_len   = 1;              /* length of current antirun */
    seq_t   k          = 1;              /* index of current antirun (1‑based) */

    while (1)
    {
        uint64_t p = next_prime(candidate);
        if (p == 0) break;               /* overflow or no more primes */

        idx_t diff = (idx_t)(p - prev);

        if (diff == 2)                   /* twin prime break ends current run */
        {
            if (curr_len == n) return k; /* first run of the requested length */

            k++;                         /* start next run */
            curr_len = 1;                /* includes prime p itself */
        }
        else
        {
            curr_len++;                  /* extend current run */
        }

        prev      = p;
        candidate = p + 1;
    }

    return 0;    /* sequence ends here (no such antirun) */
}

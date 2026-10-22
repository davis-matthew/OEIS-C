/*
 * A335185  a(n) = nextprime(ceiling(n/2)-1) - prevprime(floor(n/2)+1), where
 *          nextprime = A151800 and prevprime = A151799.
 * https://oeis.org/A335185
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nextprime(ceil(n/2)-1) - prevprime(floor(n/2)+1)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses is_prime_small for primality testing. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static inline seq_t nextprime(idx_t start)
{
    idx_t cand = start + 1;
    if (cand < 2) cand = 2;
    while (!is_prime_small((seq_t)cand))
        ++cand;
    return (seq_t)cand;
}

static inline seq_t prevprime(idx_t start)
{
    idx_t cand = start - 1;
    while (cand >= 2 && !is_prime_small((seq_t)cand))
        --cand;
    return (seq_t)cand;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;              /* Offset irrelevant for n < 4 */

    /* Compute ceil(n/2)-1 and floor(n/2)+1 */
    idx_t l = (n + 1) / 2;             /* ceil(n/2)   */
    l--;                               /* l = ceil(n/2)-1 */
    idx_t r = (n / 2) + 1;             /* r = floor(n/2)+1 */

    seq_t next = nextprime(l);
    seq_t prev = prevprime(r);
    return (seq_t)(next - prev);
}

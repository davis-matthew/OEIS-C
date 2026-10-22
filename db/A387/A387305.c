/*
 * A387305  Least k such that the Hamming weight (A000120) of n*k is prime.
 * https://oeis.org/A387305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{k ≥ 1 : popcount(n * k) is prime}
 * Range:     u8 n <= 14, u16 n <= 3854, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/popcount_seq.h"

/* Count the Hamming weight of a value of type seq_t. */

/* Simple primality test for unsigned integers up to at least 2^32. */
static inline bool is_prime_fast(unsigned n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    unsigned i = 5;
    unsigned step = 2;
    while (i * i <= n)
    {
        if (n % i == 0) return false;
        i += step;
        step = 6 - step;   /* alternates 2,4,2,4,... */
    }
    return true;
}

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 0 */
    seq_t nn = (seq_t)n;                      /* cast index to seq_t */

    unsigned pc = popcount_seq(nn);           /* popcount of n */
    if (is_prime_fast(pc)) return 1;          /* k = 1 works */

    /* Because a(n) is always odd we only need to test odd k. */
    for (idx_t k = 3; ; k += 2)
    {
        seq_t prod = nn * (seq_t)k;           /* n * k */
        unsigned pc_k = popcount_seq(prod);
        if (is_prime_fast(pc_k))
            return (seq_t)k;                  /* minimal k found */
    }
}

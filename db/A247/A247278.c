/*
 * A247278  Least integer k > 0 such that k*n - prime(k) is a square.
 * https://oeis.org/A247278
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 9003, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247278:
   a(n) = the least integer k > 0 such that k*n - prime(k) is a square.  */

/* The function implements the definition directly.  It sequentially
   tests k=1,2,... and stops when the first k satisfies the condition.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. */
    if (n < 2)
        return (seq_t)0;

    /* Search for the minimal k > 0 such that k*n - prime(k) is a square. */
    for (idx_t k = 1; ; ++k)
    {
        idx_t kn = k * n;                 /* k * n, safe in idx_t */
        uint32_t pk32 = get_prime(k);      /* prime(k) as uint32_t */
        idx_t pk  = (idx_t)pk32;           /* cast to idx_t for comparison */

        /* Once prime(k) > k*n the difference would become negative,
           and for larger k it will stay negative, so we can stop. */
        if (pk > kn)
            break;

        idx_t diff = kn - pk;              /* non‑negative difference */

        seq_t diff_seq = (seq_t)diff;      /* cast to seq_t for sqrt */
        seq_t s = floor_sqrt(diff_seq);    /* integer square root */

        if (s * s == diff_seq)             /* perfect square? */
            return (seq_t)k;               /* found the minimal k */
    }

    /* According to the conjecture the value always exists for n ≥ 2,
       so this line should never be reached.  */
    return (seq_t)0;
}

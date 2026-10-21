/*
 * A247151  Numbers n such that abs(36*n^2 - 810*n + 2753) is prime.
 * https://oeis.org/A247151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that abs(36*n^2 - 810*n + 2753) is prime
 * Range:     u8 n <= 1, u16 n <= 47, u32 n <= 52, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Static storage for up to 20000 terms – sufficient for n up to 10000 */
static seq_t seq_terms[20000];
static size_t seq_count   = 0;   /* number of terms stored */
static idx_t  next_cand   = 0;   /* next n to test for primality */

/*
 * a(n) returns the nth term of the sequence.
 * The sequence is the list of integers n ≥ 0 for which
 * |36*n^2 - 810*n + 2753| is prime.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but keep defined behaviour */

    while (seq_count < n)
    {
        /* candidate n to test */
        idx_t cand = next_cand;

        /* compute |36*n^2 - 810*n + 2753| using 64‑bit signed arithmetic */
        int64_t val  = 36LL * (int64_t)cand * (int64_t)cand
                      - 810LL * (int64_t)cand + 2753LL;
        int64_t absv = (val >= 0) ? val : -val;
        seq_t  absnum = (seq_t)absv;

        /* test primality */
        if (is_prime_big(absnum))
        {
            seq_terms[seq_count++] = (seq_t)cand;
        }

        ++next_cand;          /* advance to next integer */
    }

    return seq_terms[n - 1];
}

/*
 * A328582  Least common multiple of nonzero digits in primorial base
 *          expansion of n.
 * https://oeis.org/A328582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1161, u16 n <= 9588, u32 n <= 9588, u64 n <= 9588,
 *            u128 n <= 9588, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least common multiple of nonzero digits in primorial base expansion of n
   a(n) = LCM of nonzero digits in the primorial base expansion of n
   (OEIS A328582).  The routine evaluates the expansion directly and
   accumulates the lcm.  No memoisation is needed because the process is
   already O(k) where k is the number of primes with primorial ≤ n.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* First 16 primes – enough for all n ≤ 10 000 (primorial 13 > 10 000) */
static const uint64_t PRIMES[16] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
    17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL,
    41ULL, 43ULL, 47ULL, 53ULL
};

static const size_t NUM_PRIMES = 16;

seq_t a(idx_t n)
{
    /* Convert index to seq_t once */
    seq_t n_seq = (seq_t)n;
    seq_t lcm = 1;          /* LCM of the non‑zero digits processed so far */
    seq_t primorial = 1;    /* product of all primes processed so far */

    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        seq_t p = (seq_t)PRIMES[i];

        /* digit at the p‑ary place of the primorial expansion */
        seq_t digit = (n_seq / primorial) % p;

        if (digit != 0) {
            seq_t g = gcd(lcm, digit);
            /* lcm = lcm / g * digit;    works safely for seq_t */
            lcm = (lcm / g) * digit;
        }

        /* Stop before multiplying primorial would overflow or become > n */
        if (primorial > n_seq / p) {
            break;
        }
        primorial *= p;
    }

    return lcm;
}

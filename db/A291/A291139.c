/*
 * A291139  Primes of the form floor(k^(3/2)) for some integer k.
 * https://oeis.org/A291139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form floor(k^(3/2)) for integer k ≥
 *            2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 43, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0, safe fallback */

    /* Static state to memoize computed terms */
    static seq_t *seq = NULL;      /* array of found terms */
    static size_t seqcap = 0;      /* allocated capacity of the array */
    static size_t seqlen = 0;      /* number of terms currently stored */
    static idx_t next_k = 2;       /* next integer k to examine */

    /* Generate terms until the n-th term is available */
    while (seqlen < n) {
        /* k^3 = k * k * k, then floor(sqrt(k^3)) = floor(k^(3/2)) */
        seq_t k_seq = (seq_t)next_k;
        seq_t k3 = k_seq * k_seq * k_seq;
        seq_t val = floor_sqrt(k3);

        if (is_prime_big(val)) {   /* test primality */
            /* Grow the array if needed */
            if (seqlen == seqcap) {
                seqcap = seqcap ? seqcap * 2 : 8;
                seq = (seq_t *)realloc(seq, seqcap * sizeof(seq_t));
            }
            seq[seqlen++] = val;
        }
        ++next_k;   /* advance to next k */
    }

    return seq[n - 1];
}

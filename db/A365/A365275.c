/*
 * A365275  Number of integers k <= n that can be written as k = m^2+p^2 where
 *          p is a prime and m is a positive integer.
 * https://oeis.org/A365275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k ≤ n : ∃ prime p, m ≥ 1 such that k = p² + m² }|
 * Range:     u8 n <= 1452, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 0 */

    /* static state: bit‑mask of representable numbers up to max_seen */
    static unsigned char *seen   = NULL;   /* indices 1 … max_seen are valid */
    static idx_t          max_seen = 0;

    /* Extend computation if necessary */
    if (n > max_seen) {
        /* Allocate new array with zero‑initialisation */
        unsigned char *new_seen = (unsigned char *)
            calloc((size_t)(n + 1), sizeof(unsigned char));
        if (max_seen > 0) {
            memcpy(new_seen, seen,
                   (size_t)(max_seen + 1));
            free(seen);
        }
        seen = new_seen;

        /* Find primes up to n */
        uint64_t *prime_arr = NULL;
        size_t    prime_cnt = 0;
        build_primes_upto(n, &prime_arr, &prime_cnt);

        /* Mark all numbers k = p² + m² not yet marked */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p   = (idx_t)prime_arr[i];
            idx_t p2  = p * p;
            if (p2 > n) break;          /* primes sorted */
            for (idx_t m = 1;; ++m) {
                idx_t k = p2 + m * m;
                if (k > n) break;
                if (k > max_seen) seen[(size_t)k] = 1;
            }
        }

        free(prime_arr);
        max_seen = n;
    }

    /* Count representable numbers ≤ n */
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (seen[(size_t)i]) count += (seq_t)1;
    }
    return count;
}

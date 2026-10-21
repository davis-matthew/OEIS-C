/*
 * A277191  Number of integers k in range [n^2, ((n+1)^2)-1] for which the
 *          least number of squares that add up to k (A002828) is odd.
 * https://oeis.org/A277191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k ∈ [n², (n+1)²−1] : A002828(k) odd }
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 512, u64 n <= 512,
 *            u128 n <= 512, big any n
 * Helpers:   build_primes_upto, can_sum_two_squares, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts integers in the specified interval whose minimal number of squares is odd. */
/* It uses Legendre's theorem for representations by three squares and the prime‑factorization test for two squares. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/can_sum_two_squares.h"

seq_t a(idx_t n)
{
    /* Primes up to n+1 are sufficient for all factorizations needed. */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    idx_t req_limit = n + 1;
    if (req_limit > prime_limit) {
        /* Free old primes, if any. */
        free(prime_list);
        build_primes_upto(req_limit, &prime_list, &prime_count);
        prime_limit = req_limit;
    }

    seq_t count = 0;
    seq_t k_start = (seq_t)n * (seq_t)n;
    seq_t k_end   = ((seq_t)(n + 1) * (seq_t)(n + 1)) - 1;

    for (idx_t k_idx = (idx_t)k_start; k_idx <= (idx_t)k_end; ++k_idx) {
        seq_t k = (seq_t)k_idx;
        if (k == 0) continue;                     /* 0 has even minimal squares (0). */

        seq_t sqrtk = floor_sqrt(k);
        if (sqrtk * sqrtk == k) {                 /* one square => odd */
            count++;
            continue;
        }

        if (!can_sum_two_squares(k, prime_list, prime_count)) { /* not 2 squares */
            /* Legendre's three‑square test */
            seq_t m = k;
            while ((m & 3) == 0)                 /* divide out factors of 4 */
                m >>= 2;
            if ((m & 7) == 7)                    /* of form 4^a(8b+7) -> 4 squares (even) */
                continue;
            else                                 /* 3 squares -> odd */
                count++;
        }
        /* if it is 2 squares, parity is even – do nothing */
    }
    return count;
}

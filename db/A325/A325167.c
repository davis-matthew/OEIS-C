/*
 * A325167  Heinz number of the internal portion of the integer partition with
 *          Heinz number n.
 * https://oeis.org/A325167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Heinz of the internal portion of the partition whose
 *            Heinz number is n. The function factors the integer n into
 *            primes, reconstructs the partition by using the ordinal of each
 *            prime, builds the internal partition, and multiplies the primes
 *            corresponding to the parts of the internal partition
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, prime_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pos.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* out of range, not used by tests */

    /* Build list of primes up to n.  For n up to 10 000 this is trivial. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)n, &primes, &prime_cnt);
    if (!primes) return (seq_t)1;   /* fallback */

    /* Factor n and collect part indices (1‑based). */
    idx_t parts[64];
    idx_t parts_len = 0;
    uint64_t tmp = (uint64_t)n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p) continue;
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        for (int e = 0; e < exp; ++e) {
            parts[parts_len++] = (idx_t)(i + 1);  /* 1‑based index */
        }
    }
    if (tmp > 1) {
        /* Remaining factor is prime <= n */
        idx_t idx = prime_pos(tmp, prime_cnt, primes);
        if (idx) parts[parts_len++] = idx;
    }

    /* If partition has fewer than 2 rows, internal portion is empty. */
    seq_t result = 1;
    if (parts_len >= 2) {
        /* Build descending order of parts. */
        idx_t parts_desc[64];
        for (idx_t i = 0; i < parts_len; ++i) {
            parts_desc[i] = parts[parts_len - 1 - i];
        }
        /* Compute Heinz number of internal partition. */
        for (idx_t i = 0; i + 1 < parts_len; ++i) {
            idx_t yi = parts_desc[i];
            idx_t yi_next = parts_desc[i + 1];
            idx_t row_len = (yi - 1 < yi_next) ? (yi - 1) : yi_next;
            if (row_len > 0) {
                result = result * (seq_t)primes[row_len - 1];
            }
        }
    }

    free(primes);
    return result;
}

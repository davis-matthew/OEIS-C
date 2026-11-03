/*
 * A363752  Primes prime(k) such that prime(k) mod k is prime.
 * https://oeis.org/A363752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(k) such that prime(k) mod k is prime
 * Range:     u8 n <= 34, u16 n <= 1607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9854 (2026-11-01)
 * Helpers:   build_primes_upto, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t processed = 0;   /* number of primes already processed */
    static idx_t found = 0;        /* number of special primes found */
    static seq_t last_special = 0; /* last special prime found */
    static idx_t limit = 0;        /* current sieve limit */

    while (found < n) {
        /* Need more primes? Build a larger sieve. */
        if (processed >= prime_count) {
            idx_t new_limit;
            if (limit == 0) {
                new_limit = (n < 100) ? 100 : (idx_t)(n * 20);
            } else {
                new_limit = limit * 2;
            }

            uint64_t *tmp = NULL;
            size_t tmp_cnt = 0;
            build_primes_upto(new_limit, &tmp, &tmp_cnt);
            /* Free previous list and adopt the new one. */
            free(primes);
            primes = tmp;
            prime_count = tmp_cnt;
            limit = new_limit;
            /* Continue without resetting 'processed' – it already counts how
               many primes were processed before rebuilding. */
            continue;
        }

        uint64_t prime = primes[processed++];
        idx_t k = processed;      /* 1‑based index of this prime */
        uint64_t rem = prime % k; /* compute prime(k) mod k */

        if (rem > 1 && is_prime_seq((seq_t)rem)) {
            ++found;
            last_special = (seq_t)prime;
        }
    }
    return last_special;
}

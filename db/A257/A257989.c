/*
 * A257989  The crank of the partition having Heinz number n.
 * https://oeis.org/A257989
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = crank of the partition having Heinz number n. Compute by
 *            factorizing n into primes. For each prime factor p^e, let idx =
 *            π(p) (the 1‑based index of p in the prime sequence). Parts
 *            corresponding to p are repeated e times with index idx. Let ones
 *            = e when p = 2 (the number of 1's in the part...
 * Range:     i8 n <= 718, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;           /* undefined for offset */

    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t    cnt = 0;

    factor_u64((uint64_t)n, primes_buf, exps_buf, &cnt);

    size_t ones    = 0;
    size_t max_part = 0;
    size_t greater = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes_buf[i];
        size_t   e = exps_buf[i];

        if (p == 2) {
            ones += e;
        } else {
            size_t idx = (size_t)prime_pi(p);   /* 1‑based index of prime p */

            if (idx > max_part) max_part = idx;

            if (idx > ones) greater += e;
        }
    }

    if (ones == 0) {
        return (seq_t)max_part;
    } else {
        return (seq_t)greater - (seq_t)ones;
    }
}

/*
 * A246930  a(n) = prime(12*n).
 * https://oeis.org/A246930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(12*n)
 * Range:     u8 n <= 4, u16 n <= 545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9897 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* outside the defined offset */
    idx_t needed = (idx_t)12 * n;          /* 12*n-th prime needed */

    idx_t limit = _prime_limit_for_idx(needed);  /* safe upper bound for p_needed */

    uint64_t *prime_arr = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &prime_arr, &prime_cnt);

    /* prime_arr[0] is the first prime (2), so the 12*n-th prime is at index needed-1 */
    seq_t result = (seq_t)prime_arr[needed - 1];

    free(prime_arr);
    return result;
}

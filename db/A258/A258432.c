/*
 * A258432  Smallest number m such that A062234(m) = A062234(m-1+k) for k =
 *          1..A258383(n).
 * https://oeis.org/A258432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = start index m of the n-th maximal run of equal
 *            consecutive terms of A062234, where A062234(m) = 2*prime(m) -
 *            prime(m+1) (computed by sieve and scan)
 * Range:     u8 n <= 221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9639 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t limit = 4096;
    for (;;) {
        char *comp = (char *)calloc((size_t)limit + 1, 1);
        if (!comp) abort();
        for (uint64_t i = 2; i * i <= limit; i++) {
            if (!comp[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        /* scan primes, tracking runs of equal 2p_i - p_{i+1} */
        uint64_t prev = 0, cur = 0;      /* previous and current prime */
        int64_t prev_v = 0;
        uint64_t idx = 0;                /* 1-based index of prev prime */
        uint64_t runs = 0;
        int have_v = 0;
        for (uint64_t x = 2; x <= limit; x++) {
            if (comp[x]) continue;
            cur = x;
            if (prev) {
                int64_t v = 2 * (int64_t)prev - (int64_t)cur;
                /* v is A062234(idx) */
                if (!have_v || v != prev_v) {
                    runs++;
                    if (runs == (uint64_t)n) {
                        free(comp);
                        return (seq_t)idx;
                    }
                }
                have_v = 1;
                prev_v = v;
            }
            prev = cur;
            idx++;
        }
        free(comp);
        limit *= 2;
    }
}

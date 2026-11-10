/*
 * A346620  Partial sums of A007774.
 * https://oeis.org/A346620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A007774(k), where A007774 lists the numbers
 *            m with omega(m) = 2; computed by sieving omega and summing
 * Range:     u8 n <= 13, u16 n <= 265, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 72 terms through n = 9579 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    uint64_t limit = 64;
    while (true) {
        unsigned char *om = (unsigned char *)calloc((size_t)(limit + 1), 1);
        uint64_t p, m;
        idx_t cnt = 0;
        seq_t sum = 0;
        if (om == NULL) return 0;
        for (p = 2; p <= limit; p++) {
            if (om[p] == 0) {
                for (m = p; m <= limit; m += p) {
                    if (om[m] < 255) om[m]++;
                }
            }
        }
        for (m = 2; m <= limit; m++) {
            if (om[m] == 2) {
                cnt++;
                sum += (seq_t)m;
                if (cnt == n) {
                    free(om);
                    return sum;
                }
            }
        }
        free(om);
        limit *= 2;
    }
}

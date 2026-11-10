/*
 * A308339  Sphenic number indices of A215217.
 * https://oeis.org/A308339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k such that A007304(k) and A007304(k)+1 are both
 *            sphenic (A215217(n) = A007304(a(n))); computed by a search with
 *            trial division
 * Range:     u8 n <= 1, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 69 terms through n = 197 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool A308339_is_sphenic(seq_t num) {
    seq_t temp = num;
    int distinct_count = 0;
    for (seq_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            distinct_count++;
            if (distinct_count > 3) return false;
            temp /= p;
            if (temp % p == 0) return false;
        }
    }
    if (temp > 1) distinct_count++;
    return distinct_count == 3;
}

seq_t a(idx_t n) {
    seq_t count = 0;   /* number of sphenic numbers <= m */
    seq_t found = 0;
    bool prev = false;
    for (seq_t m = 2; ; m++) {
        bool s = A308339_is_sphenic(m);
        if (s) {
            count++;
            if (prev) {
                found++;
                if (found == (seq_t)n) return count - 1;
            }
        }
        prev = s;
    }
}

/*
 * A327433  Smallest k > 2, such that A000587(k) is divisible by n.
 * https://oeis.org/A327433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 2 with A000587(k) == 0 (mod n); A000587
 *            computed by its Aitken array reduced mod n (search)
 * Range:     u8 n <= 73, u16 n <= 4900, u32 n <= 4900, u64 n <= 4900,
 *            u128 n <= 4900, big any n
 * Validated: standard, pass, 82 terms through n = 3897 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t mod = (uint64_t)n;
    size_t cap = 256;
    size_t m = 0, k;
    uint64_t *row = (uint64_t *)malloc(cap * sizeof(uint64_t));
    seq_t result;
    row[0] = 1 % mod;
    for (;;) {
        uint64_t carry, old;
        if (m + 2 > cap) {
            cap *= 2;
            row = (uint64_t *)realloc(row, cap * sizeof(uint64_t));
        }
        carry = (mod - row[m]) % mod;
        for (k = 0; k <= m; k++) {
            old = row[k];
            row[k] = carry;
            carry = (carry + old) % mod;
        }
        row[m + 1] = carry;
        m++;
        if (m > 2 && row[0] == 0) break;
    }
    result = (seq_t)m;
    free(row);
    return result;
}

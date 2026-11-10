/*
 * A348835  Triangle, read by rows, T(n, k) is the smallest term of A006005
 *          strictly larger than abs(A006005(n) - A006005(k)).
 * https://oeis.org/A348835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   row r, col k of triangle; T(r,k) = smallest term of A006005 >
 *            |A006005(r)-A006005(k)|; computed by direct search
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static uint64_t A006005_val(uint64_t n) {
    if (n == 1) return 1;
    uint64_t count = 1, c = 3;
    while (true) {
        if (is_prime_u64(c)) {
            count++;
            if (count == n) return c;
        }
        c += 2;
    }
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t r = 1;
    while (r * (r + 1) / 2 < N) r++;
    uint64_t k = N - r * (r - 1) / 2;
    uint64_t pr = A006005_val(r), pk = A006005_val(k);
    uint64_t d = pr > pk ? pr - pk : pk - pr;
    if (d == 0) return (seq_t)1;
    uint64_t m = d + 1;
    while (true) {
        if (m >= 3 && (m & 1) && is_prime_u64(m)) return (seq_t)m;
        m++;
    }
}

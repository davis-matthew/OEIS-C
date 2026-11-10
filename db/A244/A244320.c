/*
 * A244320  a(n) = n - A014420(n).
 * https://oeis.org/A244320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A014420(n), where A014420(0) = 0 and A014420(n) = 1 +
 *            A014420(n - A081290(n)) for n > 0 (A081290(n) = largest Catalan
 *            number <= n); computed greedily
 * Range:     u8 n <= 261, u16 n <= 4862, u32 n <= 4862, u64 n <= 4862,
 *            u128 n <= 4862, big any n
 * Validated: standard, pass, 91 terms through n = 3979 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t cat[70];
    int cnt = 0;
    cat[cnt++] = 1;
    for (;;) {
        uint64_t k = (uint64_t)(cnt - 1);
        uint64_t prev = cat[k];
        uint64_t mul = 2 * (2 * k + 1);
        if (prev > UINT64_MAX / mul) break;
        uint64_t next = prev * mul / (k + 2);
        if (next > n || cnt >= 69) break;
        cat[cnt++] = next;
    }
    idx_t m = n;
    idx_t count = 0;
    while (m > 0) {
        int i = cnt - 1;
        while (i > 0 && cat[i] > m) i--;
        m -= (idx_t)cat[i];
        count++;
    }
    return (seq_t)(n - count);
}

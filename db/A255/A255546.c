/*
 * A255546  Inverse permutation to A255545.
 * https://oeis.org/A255546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = inverse of A255545. Lucky-number-style sieve: delete
 *            evens, then every 3rd, every 7th, ... (every l_p-th survivor at
 *            stage p). The m-th number deleted at stage p maps to T(p+m-1)+p,
 *            and the k-th lucky number maps to T(k), where T(k)=k(k+1)/2
 * Range:     u8 n <= 45, u16 n <= 219, u32 n <= 219, u64 n <= 219,
 *            u128 n <= 219, big any n
 * Validated: standard, pass, 97 terms through n = 214 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t tri_plus(uint64_t k, uint64_t add)
{
    seq_t K = (seq_t)k;
    return K * (K + 1) / 2 + (seq_t)add;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N % 2 == 0) {
        /* deleted at stage 1: p = 1, m = n/2 -> T(m) + 1 */
        return tri_plus(N / 2, 1);
    }
    uint64_t size = (N + 1) / 2;
    uint64_t *list = (uint64_t *)malloc((size_t)size * sizeof(uint64_t));
    if (!list) return 0;
    for (uint64_t i = 0; i < size; i++) list[i] = 2 * i + 1;
    uint64_t pos = (N + 1) / 2; /* 1-based position of n in the list */
    uint64_t p = 2;
    seq_t result = 0;
    bool done = false;
    while (p - 1 < size) {
        uint64_t l = list[p - 1];
        if (l > size) break;
        if (pos % l == 0) {
            uint64_t m = pos / l;
            result = tri_plus(p + m - 1, p);
            done = true;
            break;
        }
        uint64_t w = l - 1;
        for (uint64_t i = l - 1; i < size; i++) {
            if ((i + 1) % l != 0) list[w++] = list[i];
        }
        size = w;
        pos -= pos / l;
        p++;
    }
    if (!done) result = tri_plus(pos, 0);
    free(list);
    return result;
}

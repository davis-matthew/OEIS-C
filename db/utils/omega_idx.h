/* omega_idx: the number of distinct prime factors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_OMEGA_IDX_H
#define OEISC_UTILS_OMEGA_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* Count distinct prime factors of a positive integer x (type idx_t). */
static inline seq_t omega_idx(idx_t x) {
    if (x < 2) return 0;
    seq_t cnt = 0;
    /* handle factor 2 */
    if ((x & 1) == 0) {
        cnt++;
        while ((x & 1) == 0) x >>= 1;
    }
    /* odd factors */
    for (idx_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) {
            cnt++;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) cnt++; /* remaining prime > sqrt */
    return cnt;
}

#endif

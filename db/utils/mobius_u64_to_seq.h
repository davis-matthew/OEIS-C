/* mobius_u64_to_seq: the Moebius function mu(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MOBIUS_U64_TO_SEQ_H
#define OEISC_UTILS_MOBIUS_U64_TO_SEQ_H
#include <stdbool.h>
#include <stdint.h>

/* Möbius μ(n) for n ≥ 1. */
static seq_t mobius_u64_to_seq(uint64_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    uint64_t p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            int c = 0;
            while (n % p == 0) { n /= p; ++c; }
            if (c > 1) return 0;
            ++cnt;
        }
        ++p;
    }
    if (n > 1) ++cnt;
    return (cnt % 2 == 0) ? 1 : -1;
}

#endif

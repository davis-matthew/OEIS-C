/* bigomega_seq: the number of prime factors of n, with multiplicity.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIGOMEGA_SEQ_H
#define OEISC_UTILS_BIGOMEGA_SEQ_H
#include <stdint.h>

/* Count prime factors of m (with multiplicity) */
static seq_t bigomega_seq(seq_t m) {
    seq_t cnt = 0;
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= m; ++p) {
        while (m % p == 0) {
            ++cnt;
            m /= p;
        }
    }
    if (m > 1) ++cnt;
    return cnt;
}

#endif

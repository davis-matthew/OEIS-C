/* binomial_u64_to_seq: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_U64_TO_SEQ_H
#define OEISC_UTILS_BINOMIAL_U64_TO_SEQ_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Binomial coefficient C(n,k) using integer arithmetic (k ≤ n). */
static seq_t binomial_u64_to_seq(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)res * (n - k + i);
        res = (seq_t)(tmp / i);
    }
    return res;
}

#endif

/* triangular_idx: n(n+1)/2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TRIANGULAR_IDX_H
#define OEISC_UTILS_TRIANGULAR_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* helper: triangular sum 1+2+...+m   (m>=0) */
static inline seq_t triangular_idx(idx_t m)
{
    /* Compute m*(m+1)/2 without overflow by dividing first */
    if (m & 1)        /* m odd -> divide (m+1) by 2 */
        return (seq_t)m * ((seq_t)(m + 1) >> 1);
    else             /* m even -> divide m by 2 */
        return ((seq_t)m >> 1) * (seq_t)(m + 1);
}

#endif

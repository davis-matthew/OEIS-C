/*
 * A243815  Number of length n words on alphabet {0,1} such that the length of
 *          every maximal block of 0's (runs) is the same.
 * https://oeis.org/A243815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{r≥1} Σ_{k≥1, n−r·k≥r−1} binom(n − r·k + 1, r)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 14, ... */
#include <stdint.h>

static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t mult = n - k + i;               /* term to multiply by */
        if (sizeof(seq_t) <= 8) {             /* 64‑bit or smaller */
            unsigned __int128 tmp = (unsigned __int128)res * (unsigned __int128)mult;
            tmp /= (unsigned __int128)i;
            res = (seq_t)tmp;
        } else { /* arbitrary‑precision seq_t */
            res = (res * (seq_t)mult) / (seq_t)i;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 1; /* all‑ones word */
    for (idx_t r = 1; r <= n; ++r) {
        if (r > n - r + 1) break;                 /* maxk would be zero */
        idx_t maxk = (n - r + 1) / r;
        if (maxk == 0) break;
        for (idx_t k = 1; k <= maxk; ++k) {
            idx_t m = n - r * k + 1;
            total += binom(m, r);
        }
    }
    return total;
}

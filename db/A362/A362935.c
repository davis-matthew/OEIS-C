/*
 * A362935  a(n) = (A000982(n) - A132188(n))/2.
 * https://oeis.org/A362935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000982(n) – A132188(n)) / 2 where A000982(n) counts
 *            ordered pairs (i,j)∈[1..n]² with integer arithmetic mean and
 *            A132188(n) counts ordered pairs with integer geometric mean. The
 *            algorithm enumerates all square‑free numbers f≤n and uses
 *            floor_sqrt(n/f) to count how many i and j have the s...
 * Range:     u8 n <= 22, u16 n <= 447, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8575 (2026-11-01)
 * Helpers:   build_spf, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 0, but harmless */

    /* arithmetic mean pairs: i and j have the same parity */
    seq_t evens = (seq_t)(n / 2);
    seq_t odds  = (seq_t)(n - (n / 2));
    seq_t arithmetic = evens * evens + odds * odds;

    /* build smallest prime factor array to test square‑freeness */
    idx_t *spf = NULL;
    build_spf(n, &spf);

    seq_t geom_sum = 0;
    for (idx_t f = 1; f <= n; ++f) {
        /* test whether f is square‑free */
        bool squarefree = true;
        idx_t x = f;
        while (x > 1) {
            idx_t p = spf[x];
            x /= p;
            if (x % p == 0) {         /* p appears at least twice */
                squarefree = false;
                break;
            }
        }
        if (!squarefree) continue;

        seq_t div = (seq_t)n / (seq_t)f;
        seq_t s   = floor_sqrt(div);
        geom_sum += s * s;
    }

    free(spf);

    return (arithmetic - geom_sum) / 2;
}

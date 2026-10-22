/*
 * A386318  a(n) = the minimum value of (x + 2)*(y + 2) such that x*y = n.
 * https://oeis.org/A386318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min_{x*y = n} (x+2)*(y+2), with a(0)=4
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the minimum value (x+2)*(y+2) over all factor pairs x*y=n.
    For n≥1, the value is always >0, so we can initialise an accumulator
    with 0 and replace it with the first candidate on first hit. */

/*  This implementation factors n, then enumerates its divisors recursively. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void dfs_divisors(uint64_t cur, size_t idx,
                         const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, uint64_t n, seq_t *min_val)
{
    if (idx == cnt) {
        uint64_t other = n / cur;
        seq_t val = ((seq_t)cur + 2) * ((seq_t)other + 2);
        /* initialise(min_val==0) or update */
        if (*min_val == 0 || val < *min_val)
            *min_val = val;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t prod = 1;
    for (uint64_t i = 0; i <= ex[idx]; ++i) {
        dfs_divisors(cur * prod, idx + 1, pr, ex, cnt, n, min_val);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    /* a(0) = 4 (2×2) */
    if (n == 0)
        return (seq_t)4;

    uint64_t nn = (uint64_t)n;

    /* Prepare buffers for factorisation. Max distinct primes for 64‑bit
       numbers is 16; use 20 for safety. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t min_val = 0;   /* 0 indicates not yet set */

    dfs_divisors(1, 0, primes, exps, cnt, nn, &min_val);

    return min_val;
}

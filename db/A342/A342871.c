/*
 * A342871  a(n) = Sum_{k=1..n} floor(n^(1/k)), n >= 1.
 * https://oeis.org/A342871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} floor(n^(1/k))
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * Compute floor(log2(x)) for unsigned 64‑bit x.
 * Returns 0 for x == 0 (unused for n >= 1).
 */
static inline idx_t floor_log2_uint64(idx_t x)
{
    return x == 0 ? 0 : 63 - __builtin_clzll(x);
}

/*
 * Compute integer k‑th root of n → largest r with r^k <= n.
 * n fits in idx_t and r^k <= n <= 2^64‑1, so intermediate
 * products can be safely managed using seq_t without overflow.
 */
static seq_t kth_root(idx_t n, idx_t k)
{
    seq_t lo = (seq_t)1;
    seq_t hi = (seq_t)n;
    seq_t n_seq = (seq_t)n;

    while (lo < hi) {
        seq_t mid = (lo + hi + (seq_t)1) / (seq_t)2;

        /* Compute mid^k with early exit if exceeds n_seq */
        seq_t p = (seq_t)1;
        bool ok = true;
        for (idx_t i = 0; i < k; ++i) {
            if (p > n_seq / mid) {  /* would overflow or exceed n */
                ok = false;
                break;
            }
            p *= mid;
        }

        if (ok && p <= n_seq)
            lo = mid;
        else
            hi = mid - (seq_t)1;
    }
    return lo;
}

/*
 * Sequence a(n) = Sum_{k=1..n} floor(n^(1/k)), with a(1)=1.
 * The algorithm uses the observation that for k > floor(log2(n)),
 * floor(n^(1/k)) == 1, so the sum can be split into a short loop
 * (k <= floor(log2(n))) plus a linear tail of 1's.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined per OEIS offset, but safe */

    idx_t m = floor_log2_uint64(n);
    seq_t sum = 0;

    for (idx_t k = 1; k <= m; ++k)
        sum += kth_root(n, k);

    /* tail of 1's for k from m+1 to n */
    sum += (seq_t)(n - m);

    return sum;
}

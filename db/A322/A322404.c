/*
 * A322404  Square array T(n, k) read by antidiagonals, n >= 0 and k >= 0: the
 *          lengths of runs in binary expansion of T(n, k) are obtained by
 *          adding those of n and of k (see Comments for precise definition).
 * https://oeis.org/A322404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd_size_t, lcm_size_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A322404
 *  a(n) = T(i , j) where d = ⌊(√(8n+1)-1)/2⌋,
 *         i = n - d(d+1)/2,  j = d - i.
 *  T is defined by run‑length addition of periodic sequences
 *  derived from the binary expansions of the arguments.
 *  Run lengths of 0 are defined to be (0).
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd_size_t.h"
#include "../utils/lcm_size_t.h"

/* ---------- utilities ---------- */

/* ---------- run‑length extraction ---------- */

/* Extract run lengths of x into array a[128].
 * *cnt will contain the number of runs.
 *   For x == 0, a[0] = 0 and *cnt = 1.
 */
static void run_lengths(seq_t x, unsigned a[128], size_t *cnt)
{
    if (x == 0) {
        a[0] = 0;
        *cnt = 1;
        return;
    }

    /* Determine number of bits */
    size_t bits = 0;
    seq_t tmp = x;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }

    unsigned prev = 1;      /* first run is of 1s */
    unsigned run  = 0;
    *cnt = 0;

    for (long long i = (long long)bits - 1; i >= 0; --i) {
        unsigned bit = ((x >> (unsigned)i) & 1) != 0;
        if (bit == prev) {
            ++run;
        } else {
            a[(*cnt)++] = run;
            prev = bit;
            run  = 1;
        }
    }
    a[(*cnt)++] = run;     /* final run */
}

/* ---------- T(n, k) by run‑length addition ---------- */

static seq_t T(seq_t a, seq_t b)
{
    unsigned ra[128], rb[128];
    size_t na, nb;

    run_lengths(a, ra, &na);
    run_lengths(b, rb, &nb);

    size_t l = lcm_size_t(na, nb);        /* number of runs to take */

    seq_t res = 0;

    for (size_t i = 0; i < l; ++i) {
        unsigned sum = ra[i % na] + rb[i % nb];

        /* shift left by 'sum' bits, one shift at a time to avoid UB */
        for (unsigned j = 0; j < sum; ++j) {
            res <<= 1;
        }

        /* if this run is of 1s (even index), add ones */
        if ((i & 1) == 0) {
            seq_t mask = 0;
            for (unsigned j = 0; j < sum; ++j) {
                mask = (mask << 1) | (seq_t)1;
            }
            res |= mask;
        }
    }
    return res;
}

/* ---------- find diagonal d for index n ---------- */

static uint64_t diagonal_of(idx_t n)
{
    uint64_t lo = 0, hi = 1;
    /* Find an upper bound for d */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) {
        hi <<= 1;
        if (hi > UINT64_MAX / 2) { hi = UINT64_MAX; break; }
    }

    /* Binary search in [lo, hi] */
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    uint64_t d = diagonal_of(n);
    unsigned __int128 triangular = (unsigned __int128)d * (d + 1) / 2;

    seq_t i = (seq_t)(n - (idx_t)triangular);
    seq_t j = (seq_t)(d - (uint64_t)i);

    return T(i, j);
}

/*
 * A380105  Perimeter-magic triangles of order 3 with magic sum n, bracelet
 *          symmetry, and minimum term 1.
 * https://oeis.org/A380105
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A380853(n) - A380853(n-3) (Perimeter‑magic triangles of
 *            order 3 with minimum term 1, offset 9 – a(9) is the first term).
 *            The sequence satisfies the linear recurrence a(n) = -a(n‑1) +
 *            a(n‑3) + 2a(n‑4) + 2a(n‑5) + a(n‑6) -a(n‑7) -2a(n‑8) -2a(n‑9)
 *            -a(n‑10) + a(n‑12) + a(n‑13) for n ≥ 14. Init...
 * Range:     u8 n <= 33, u16 n <= 425, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"

#define CACHE_LIMIT 100000          /* upper bound for n in tests */

static const seq_t init_vals[13] = {
    (seq_t)1,  (seq_t)3,  (seq_t)5,  (seq_t)12,  (seq_t)11,
    (seq_t)20, (seq_t)24, (seq_t)33, (seq_t)33, (seq_t)52,
    (seq_t)51, (seq_t)68, (seq_t)70
};

static const int Ce[14] = {
    0,  -1,  0,   1,   2,   2,   1,  -1,  -2,  -2,  -1,  0,   1,   1
};

static seq_t cache[CACHE_LIMIT + 1] = {};          /* static storage */
static idx_t cache_max = 8;                       /* highest index stored */

/* Ensure that cache contains values up to n (and that n < CACHE_LIMIT). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;
    if (n > CACHE_LIMIT) n = CACHE_LIMIT;        /* truncate if too large */

    /* Initialise initial values if not already done */
    if (cache_max < 21) {
        for (idx_t i = 9; i <= 21 && i <= n; ++i) {
            cache[i] = init_vals[i - 9];
        }
        cache_max = (cache_max < 21 ? 21 : cache_max);
    }

    /* Compute forward using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 1; k <= 13; ++k) {
            int coeff = Ce[k];
            if (coeff == 0) continue;
            seq_t prod = mul_small(cache[i - k],
                                   (unsigned int)(coeff > 0 ? coeff : -coeff));
            if (coeff > 0)
                pos += prod;
            else
                neg += prod;
        }
        cache[i] = pos - neg;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 9) return 0;
    if (n > CACHE_LIMIT) {
        /* Fallback: compute with a small rolling buffer (no memoisation) */
        seq_t buf[13] = {};
        /* initialise last 13 values using known initial segment */
        for (int i = 0; i < 13; ++i)
            buf[i] = init_vals[i];               /* a(9)..a(21) */
        idx_t cur = 21;
        for (idx_t i = cur + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            for (int k = 1; k <= 13; ++k) {
                int coeff = Ce[k];
                if (coeff == 0) continue;
                seq_t prod = mul_small(buf[(k-1)%13],
                                       (unsigned int)(coeff > 0 ? coeff : -coeff));
                if (coeff > 0) pos += prod;
                else neg += prod;
            }
            buf[(i-9)%13] = pos - neg;
        }
        return buf[(n-9)%13];
    }
    ensure_cache(n);
    return cache[n];
}

/*
 * A338119  Positive integers not congruent to 0 or 2 modulo 8 which cannot be
 *          written as x^2 + y^2 + z^2 + w^2 with x + y + 2*z a positive power
 *          of 4, where x, y, z, w are nonnegative integers.
 * https://oeis.org/A338119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer not congruent to 0 or 2 modulo 8 *
 *            that cannot be represented as x^2 + y^2 + z^2 + w^2 with * x + y
 *            + 2*z a positive power of 4. The sequence is computed by a *
 *            direct search with a pre‑computed table of two‑square sums
 * Range:     u8 n <= 14, u16 n <= 124, u32 n <= 124, u64 n <= 124,
 *            u128 n <= 124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* maximum n up to which the pre‑computed table is built;
   the known list ends at 10839, so 200000 is far enough. */
#define MAX_LIMIT 200000
/* number of 64‑bit words needed to store sums up to 2*sqrt(MAX_LIMIT) */
#define WORDS ((2 * (unsigned int)sqrt((double)MAX_LIMIT) + 63) / 64)

/* pre‑computed bitsets: pairbits[sum_sq * WORDS + k] contains bits for
   all possible x+y values that yield sum_sq = x^2 + y^2. */
static unsigned long long *pairbits = NULL;
static bool precomp_done = false;

/* integer square root, safe for up to 64‑bit values in our range. */
static inline unsigned int u64_sqrt(unsigned long long n)
{
    unsigned long long r = (unsigned long long)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return (unsigned int)r;
}

/* build the pairbits table once */
static void init_precomp(void)
{
    if (precomp_done) return;

    size_t sz = (size_t)(MAX_LIMIT + 1) * WORDS;
    pairbits = (unsigned long long *)calloc(sz, sizeof(unsigned long long));
    if (!pairbits) exit(1);               /* safety fall-back */

    unsigned int max_base = u64_sqrt(MAX_LIMIT);
    for (unsigned int x = 0; x <= max_base; ++x) {
        unsigned int xsq = x * x;
        for (unsigned int y = 0; y <= max_base; ++y) {
            unsigned int sq = xsq + y * y;
            if (sq > MAX_LIMIT) break;
            unsigned int sum_xy = x + y;
            size_t idx = (size_t)sq * WORDS + (sum_xy >> 6);
            pairbits[idx] |= 1ULL << (sum_xy & 63);
        }
    }
    precomp_done = true;
}

/* test whether n can be written as x^2+y^2+z^2+w^2 with
   x+y+2*z a positive power of 4. */
static bool has_good_repr(seq_t n)
{
    unsigned long long N = (unsigned long long)n;

    for (unsigned int z = 0; (unsigned long long)z * z <= N; ++z) {
        unsigned long long z2   = (unsigned long long)z * z;
        unsigned long long restZ= N - z2;
        for (unsigned int w = 0; (unsigned long long)w * w <= restZ; ++w) {
            unsigned long long w2    = (unsigned long long)w * w;
            unsigned long long rem   = restZ - w2;
            unsigned int  sqrt_rem   = u64_sqrt(rem);
            unsigned long long max_p = (unsigned long long)2 * sqrt_rem + 2ULL * z;
            if (max_p < 4) continue;         /* smallest power of 4 is 4 */
            for (unsigned long long p = 4; p <= max_p; p <<= 2) {
                unsigned long long s_xy = p - 2ULL * z;   /* required x+y */
                if (s_xy > 2ULL * sqrt((double)MAX_LIMIT)) continue;
                size_t idx = (size_t)rem * WORDS + (s_xy >> 6);
                if ((pairbits[idx] >> (s_xy & 63)) & 1ULL) {
                    return true;                /* valid representation found */
                }
            }
        }
    }
    return false;    /* no representation with required sum found */
}

/* dynamic cache of the sequence */
static seq_t *seq_arr     = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static unsigned long long next_candidate = 1;

/* main sequence function */
seq_t a(idx_t n)
{
    init_precomp();

    while (seq_len < (size_t)n) {
        /* skip numbers congruent to 0 or 2 modulo 8 */
        while ((next_candidate & 7ULL) == 0 || (next_candidate & 7ULL) == 2)
            ++next_candidate;

        seq_t cand = (seq_t)next_candidate;
        if (!has_good_repr(cand)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_arr = tmp;
                seq_cap = new_cap;
            }
            seq_arr[seq_len++] = cand;
        }
        ++next_candidate;
    }
    return seq_arr[n - 1];
}

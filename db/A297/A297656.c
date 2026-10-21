/*
 * A297656  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 3 neighboring 1s.
 * https://oeis.org/A297656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n‑1) +67*a(n‑3) +65*a(n‑4) +58*a(n‑5) +60*a(n‑6)
 *            -296*a(n‑7) -269*a(n‑8) -48*a(n‑9) +43*a(n‑10) +400*a(n‑11)
 *            +473*a(n‑12) +81*a(n‑13) -9*a(n‑14) +23*a(n‑15) +a(n‑16)
 *            +4*a(n‑17) for n > 17; initial values: a(1)=1, a(2)=36,
 *            a(3)=231, a(4)=1073, a(5)=6838, a(6)=45036, a(7)=268655,
 *            a(8)=16...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_vals[18] = {
    0ULL,
    1ULL, 36ULL, 231ULL, 1073ULL, 6838ULL,
    45036ULL, 268655ULL, 1617465ULL, 9996907ULL,
    61298132ULL, 373667037ULL, 2285559013ULL,
    13993745417ULL, 85580111107ULL, 523343997645ULL,
    3201466965125ULL, 19583433034979ULL
};

static const int coeffs[17] = {
    4,  0, 67, 65, 58, 60, -296, -269, -48, 43,
    400, 473, 81, -9, 23, 1, 4
};

static seq_t *cache = NULL;      /* array indexed by n, cache[0] unused */
static size_t cache_cap = 0;     /* capacity of cache (number of valid indices) */

static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    size_t needed = (size_t)n + 1;          /* need indices 1..n inclusive */
    if (cache_cap >= needed) return;

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) return;                /* allocation failed */
    cache = new_cache;

    if (cache_cap == 0) {
        /* first time: copy initial values up to 17 or needed */
        for (size_t i = 1; i < needed && i < 18; ++i)
            cache[i] = base_vals[i];
        for (size_t i = 18; i < needed; ++i) {
            seq_t pos = 0, neg = 0;
            for (int k = 1; k <= 17; ++k) {
                int c = coeffs[k-1];
                if (!c) continue;
                seq_t term = cache[i - k] * (seq_t)(c > 0 ? c : -c);
                if (c > 0) pos += term;
                else       neg += term;
            }
            cache[i] = pos - neg;
        }
    } else {
        /* copy any missing initial values if we had less than 18 stored */
        for (size_t i = cache_cap; i < needed && i < 18; ++i)
            cache[i] = base_vals[i];
        /* compute new terms */
        size_t start = (cache_cap < 18) ? 18 : cache_cap;
        for (size_t i = start; i < needed; ++i) {
            seq_t pos = 0, neg = 0;
            for (int k = 1; k <= 17; ++k) {
                int c = coeffs[k-1];
                if (!c) continue;
                seq_t term = cache[i - k] * (seq_t)(c > 0 ? c : -c);
                if (c > 0) pos += term;
                else       neg += term;
            }
            cache[i] = pos - neg;
        }
    }

    cache_cap = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0ULL;             /* undefined for n<1 */
    ensure_cache(n);
    if (!cache) return 0ULL;             /* allocation failed */
    return cache[n];
}

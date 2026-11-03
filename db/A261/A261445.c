/*
 * A261445  Expansion of f(x, x^3) * f(x, x^2)^3 in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A261445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = (1/n) * Σ_{k=1..n} S(k) * a(n−k) where
 *            S(k) = Σ_{d|k} d * C(d) and the periodic coefficient C(d) = {4,
 *            -1, -2, -1, 4, -4} for d ≡ 1,2,3,4,5,0 (mod 6)
 * Range:     u8 n <= 7, u16 n <= 238, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 973 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* periodic coefficient C(d) for divisors d. */
static int coeff_of(idx_t d)
{
    switch (d % 6) {
        case 1: return  4;
        case 2: return -1;
        case 3: return -2;
        case 4: return -1;
        case 5: return  4;
        case 0: return -4;
    }
    return 0;               /* unreachable */
}

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;   /* cached sequence values a(0)…a(N) */
    static int64_t *s    = NULL;   /* cached S(k) values */
    static size_t sz     = 0;      /* current maximum cached index */

    if ((size_t)n >= sz) {
        size_t new_sz = (size_t)n + 1;

        seq = mem_resize(seq_t, seq, new_sz);
        s   = (int64_t *)realloc(s,   new_sz * sizeof(int64_t));

        /* initialize new slots */
        for (size_t i = sz; i < new_sz; ++i) {
            seq[i] = 0;
            s[i]   = 0;
        }

        /* compute S(k) for new indices */
        for (size_t k = 1; k < new_sz; ++k) {
            int64_t sum = 0;
            for (size_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    sum += (int64_t)d * coeff_of((idx_t)d);
                    size_t e = k / d;
                    if (e != d)
                        sum += (int64_t)e * coeff_of((idx_t)e);
                }
            }
            s[k] = sum;
        }

        /* first term */
        if (sz == 0) seq[0] = 1;

        /* compute sequence values up to new_sz-1 */
        for (size_t idx = sz; idx < new_sz; ++idx) {
            if (idx == 0) continue; /* already set */
            seq_t pos = 0;
            seq_t neg = 0;
            for (size_t k = 1; k <= idx; ++k) {
                int64_t coeff = s[k];
                if (coeff >= 0)
                    pos = pos + mul_small(seq[idx - k], (unsigned)coeff);
                else
                    neg = neg + mul_small(seq[idx - k], (unsigned)(-coeff));
            }
            seq_t sum = pos - neg;                 /* pos ≥ neg */
            seq[idx] = sum / (seq_t)idx;           /* division exact */
        }

        sz = new_sz;
    }
    return seq[n];
}

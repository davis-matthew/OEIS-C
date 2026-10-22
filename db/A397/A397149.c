/*
 * A397149  G.f. A(x), with A(0) = 0, satisfies A(x-x^3) = x + A(x)^4.
 * https://oeis.org/A397149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A397149
 *  Recurrence:
 *      a(1) = 1
 *      a(n) = Σ_{i+j+k+l=n} a(i)a(j)a(k)a(l)
 *            + Σ_{r=1..⌊n/3⌋} (-1)^{r+1} * binomial(n-2r, r) * a(n-2r)
 *  (All indices are ≥1.  a(0) is unused; returns 0 for n == 0.)
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static buffers for memoisation */
    static seq_t *aval = NULL;   /* a[1 .. current_max] */
    static seq_t *pair = NULL;   /* p[2 .. current_max],  p[k] = Σ_{i+j=k} a(i)a(j) */
    static idx_t  a_len = 0;     /* largest n for which a[n] is computed */
    static size_t a_cap = 0;     /* allocated capacity for aval */
    static size_t p_cap = 0;     /* allocated capacity for pair */

    /* a(0) is not used in the sequence */
    if (n == 0)
        return (seq_t)0;

    /* Ensure enough space for aval and pair up to n */
    if (a_cap <= n) {
        size_t new_cap = n * 2 + 10;
        aval = (seq_t *)realloc(aval, new_cap * sizeof(seq_t));
        memset(aval + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
        a_cap = new_cap;
    }
    if (p_cap <= n) {
        size_t new_p = n * 2 + 10;
        pair = (seq_t *)realloc(pair, new_p * sizeof(seq_t));
        memset(pair + p_cap, 0, (new_p - p_cap) * sizeof(seq_t));
        p_cap = new_p;
    }

    /* Initialise a(1) on first use */
    if (a_len == 0) {
        aval[1] = (seq_t)1;
        /* p[2] = a1 * a1 after setting a(1) */
        pair[2] = aval[1] * aval[1];
        a_len = 1;
    }

    /* Compute missing terms up to requested n */
    for (idx_t k = a_len + 1; k <= n; ++k) {
        /* 1. 4‑fold convolution part: Σ_{m=2}^{k-2} p[m] * p[k-m] */
        seq_t conv4 = 0;
        if (k >= 4) {
            for (idx_t m = 2; m <= k - 2; ++m) {
                conv4 += pair[m] * pair[k - m];
            }
        }

        /* 2. Alternating binomial part */
        seq_t alt = 0;
        for (idx_t r = 1; r <= k / 3; ++r) {
            seq_t bin = binomial(k - 2 * r, r);
            seq_t term = bin * aval[k - 2 * r];
            if (r & 1)
                alt += term;   /* r odd  → +(r+1) even → + */
            else
                alt -= term;   /* r even → -(r+1) odd  → – */
        }

        /* 3. Store a(k) */
        aval[k] = conv4 + alt;

        /* 4. Compute p[k] = Σ_{i=1}^{k-1} a(i)a(k-i)  (k ≥ 2) */
        if (k >= 2) {
            seq_t sum = 0;
            for (idx_t i = 1; i < k; ++i) {
                sum += aval[i] * aval[k - i];
            }
            pair[k] = sum;
        }
    }

    a_len = n;
    return aval[n];
}

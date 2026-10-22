/*
 * A398978  Expansion of e.g.f. 1/(1 - sin(x) - sin(x)^2).
 * https://oeis.org/A398978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k * binomial(n,2*k+1) *
 *            a(n-2*k-1) + 2 * Sum_{k=1..floor(n/2)} (-4)^(k-1) *
 *            binomial(n,2*k) * a(n-2*k) Initial term: a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build; do not define them. */
extern seq_t binomial(idx_t n, idx_t k);   /* provided helper */

/* The function computing the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *a_cache = NULL;   /* array: a_cache[i] = a(i) */
    static size_t a_len = 0;        /* number of cached entries */

    /* Cache for powers of 4: pow4[e] = 4^e (e >= 0) */
    static seq_t *pow4 = NULL;
    static size_t pow4_len = 0;

    /* Initialise caches on first call. */
    if (a_len == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cache[0] = 1;      /* a(0) = 1 */
        a_len = 1;
    }
    if (pow4_len == 0) {
        pow4 = (seq_t *)malloc(sizeof(seq_t));
        pow4[0] = 1;         /* 4^0 = 1 */
        pow4_len = 1;
    }

    /* Compute terms up to requested n. */
    while (a_len <= n) {
        idx_t cur = a_len;     /* index of term to compute */

        /* Ensure we have enough powers of 4.  The largest exponent needed
           for this cur is e_max = cur/2 - 1 (for k = cur/2).  We thus
           need pow4_len > e_max, i.e. pow4_len >= cur/2 + 1. */
        idx_t needed_pow4 = cur / 2 + 1;
        if (pow4_len < needed_pow4) {
            size_t new_len = pow4_len;
            while (new_len < needed_pow4) new_len <<= 1;
            pow4 = (seq_t *)realloc(pow4, new_len * sizeof(seq_t));
            for (size_t i = pow4_len; i < new_len; ++i)
                pow4[i] = pow4[i - 1] * 4;
            pow4_len = new_len;
        }

        /* Compute a[cur] using the recurrence. */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        seq_t bin = 1; /* binomial(cur, 0) */
        for (idx_t j = 1; j <= cur; ++j) {
            /* Update binomial coefficient to binomial(cur, j) */
            bin = bin * (cur - j + 1) / j;

            if (j & 1) {   /* j is odd  => first sum */
                idx_t k = (j - 1) >> 1;        /* k = (j-1)/2 */
                seq_t term = bin * a_cache[cur - j];
                if ((k & 1) != 0) { /* (-1)^k negative when k odd */
                    sum_neg += term;
                } else {
                    sum_pos += term;
                }
            } else {        /* j is even  => second sum, k=j/2 */
                idx_t k = j >> 1;
                if (k == 0) continue;           /* j=0 cannot happen here */
                idx_t e = k - 1;                 /* exponent of (-4) */
                seq_t term = bin * a_cache[cur - j];
                term *= pow4[e];                 /* multiply by 4^e */
                term *= (seq_t)2;                /* final factor 2 */
                if ((e & 1) != 0) { /* (-4)^e negative when e odd */
                    sum_neg += term;
                } else {
                    sum_pos += term;
                }
            }
        }

        /* The result must be non‑negative.  Compute difference. */
        seq_t val;
        if (sum_pos >= sum_neg) {
            val = sum_pos - sum_neg;
        } else {
            /* In theory this should not happen; for safety we still compute. */
            val = sum_neg - sum_pos;
        }

        /* Store the computed term. */
        a_cache = (seq_t *)realloc(a_cache, (a_len + 1) * sizeof(seq_t));
        a_cache[a_len] = val;
        ++a_len;
    }

    return a_cache[n];
}

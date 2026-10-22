/*
 * A317379  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with the king‑move adjacency
 *            restriction described in OEIS A317379. For n ≥ 24 the sequence
 *            satisfies the linear recurrence from the OEIS entry, and the
 *            first 24 terms are used as seed values
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The array is stored 1‑based; a(1) is at cache[1].  The cache
   grows up to the maximum n requested (assumed ≤ 20000 for
   performance).  The recurrence coefficients are applied to the
   appropriate previous terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Initial 24 terms – mandatory seeds, not a lookup cache. */
static const seq_t init_vals[24] = {
    (seq_t)8ULL,          /* a(1)  */
    (seq_t)9ULL,          /* a(2)  */
    (seq_t)36ULL,         /* a(3)  */
    (seq_t)123ULL,        /* a(4)  */
    (seq_t)414ULL,        /* a(5)  */
    (seq_t)1580ULL,       /* a(6)  */
    (seq_t)5704ULL,       /* a(7)  */
    (seq_t)20162ULL,      /* a(8)  */
    (seq_t)72715ULL,      /* a(9)  */
    (seq_t)262549ULL,     /* a(10) */
    (seq_t)944628ULL,     /* a(11) */
    (seq_t)3399508ULL,    /* a(12) */
    (seq_t)12242821ULL,   /* a(13) */
    (seq_t)44087164ULL,   /* a(14) */
    (seq_t)158739670ULL,  /* a(15) */
    (seq_t)571576721ULL,  /* a(16) */
    (seq_t)2058142185ULL, /* a(17) */
    (seq_t)7410913041ULL, /* a(18) */
    (seq_t)26684956759ULL,/* a(19) */
    (seq_t)96086522975ULL,/* a(20) */
    (seq_t)345986123562ULL,/* a(21) */
    (seq_t)1245818108353ULL,/* a(22) */
    (seq_t)4485910314885ULL,/* a(23) */
    (seq_t)16152754437159ULL/* a(24) */
};

/* Recurrence coefficients for n > 23:
   coeff[i] = coefficient of a(n-i-1)  (i = 0 … 18)
   The recurrence is:  a(n) = Σ coeff[i] * a(n-i-1)      (n > 23) */
static const int coeff[19] = {
    2,  6,  4, -6, -32, -43,  7, 82, 59, 16,
   10, -62, -69, -24, -7,  24, 34,  8, -8
};

/* Cache: allocated once, size 20001 (1‑based indexing). */
static seq_t cache[20001];   /* indices 1 … 20000 */
static size_t cache_len = 0; /* highest index computed */

/* Ensure that values up to n are computed and stored. */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= cache_len) return;
    if (n >= sizeof(cache)/sizeof(cache[0])) {
        /* For safety, clamp to the fixed cache size. */
        return;
    }

    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 24) {
            cache[i] = init_vals[i - 1];
        } else {
            /* Apply the recurrence. */
            seq_t sum = 0;
            for (int k = 0; k < 19; ++k) {
                seq_t val = cache[i - k - 1];
                int c = coeff[k];
                if (c >= 0)
                    sum += (seq_t)c * val;
                else
                    sum -= (seq_t)(-c) * val;
            }
            cache[i] = sum;
        }
    }
    cache_len = (size_t)n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for 0 or negative */
    ensure_cache(n);
    return cache[(size_t)n];
}

/*
 * A394015  a(n) = (2*n)! * [x^(2*n)] cos(x) * cos(2*x) / cos(4*x).
 * https://oeis.org/A394015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * (9^n + 1)/2 - Sum_{k=0..n-1} (-16)^(n-k) *
 *            binomial(2*n,2*k) * a(k) Offset: 0 Initial values: a(0)=1,
 *            a(1)=11, a(2)=841,
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Forward declaration of the supplied binomial helper */
seq_t binomial(idx_t a, idx_t b);

/* Static storage for memoised sequence values */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap  = 0;
static idx_t   a_cache_len = 0;

/* Static storage for powers of 9 and 16 */
static seq_t *pow9_cache   = NULL;
static size_t pow9_cap     = 0;
static size_t pow9_len     = 0;   /* number of elements actually computed */

static seq_t *pow16_cache  = NULL;
static size_t pow16_cap    = 0;
static size_t pow16_len    = 0;

/* Ensure a dynamic array has room for at least `need` elements. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
#ifdef __cplusplus
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
#else
        *arr = realloc(*arr, new_cap * sizeof(seq_t));
#endif
        *cap = new_cap;
    }
}

/* Compute the n-th term of sequence A394015. */
seq_t a(idx_t n)
{
    /* Make sure the memoisation arrays can hold indices up to n */
    ensure_capacity(&a_cache,   &a_cache_cap,   n + 1);
    ensure_capacity(&pow9_cache, &pow9_cap,      n + 1);
    ensure_capacity(&pow16_cache,&pow16_cap,     n + 1);

    /* Initialise the power tables if they are empty */
    if (pow9_len == 0) {
        pow9_cache[0] = (seq_t)1;
        pow9_len = 1;
    }
    if (pow16_len == 0) {
        pow16_cache[0] = (seq_t)1;
        pow16_len = 1;
    }

    /* Compute missing powers of 9 up to n */
    while (pow9_len <= (size_t)n) {
        pow9_cache[pow9_len] = pow9_cache[pow9_len - 1] * (seq_t)9;
        ++pow9_len;
    }

    /* Compute missing powers of 16 up to n */
    while (pow16_len <= (size_t)n) {
        pow16_cache[pow16_len] = pow16_cache[pow16_len - 1] * (seq_t)16;
        ++pow16_len;
    }

    /* Compute any pending terms of the sequence up to n */
    for (idx_t cur = a_cache_len; cur <= n; ++cur) {
        /* base term: (9^cur + 1)/2 with sign (-1)^cur */
        seq_t base_raw = (pow9_cache[cur] + (seq_t)1) / (seq_t)2;
        int base_neg   = (cur & 1) != 0;   /* true if (-1)^cur == -1 */

        /* Accumulate positive and negative contributions from the sum */
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;

        idx_t m = 2 * cur;            /* 2*cur for the binomial index */
        seq_t binom = (seq_t)1;       /* C(m, 0) */

        for (idx_t k = 0; k < cur; ++k) {
            idx_t t     = cur - k;                 /* exponent in (-16)^t */
            seq_t term   = pow16_cache[t] * binom; /* 16^t * C(2*cur, 2*k) */
            term = term * a_cache[k];

            if (t & 1) {
                sum_neg += term;
            } else {
                sum_pos += term;
            }

            /* Update binom for next k: C(m, 2(k+1)) from C(m, 2k) */
            if (k + 1 < cur) {
                seq_t mul1 = (seq_t)(m - 2 * k);
                seq_t mul2 = (seq_t)(m - 2 * k - 1);
                seq_t div1 = (seq_t)(2 * k + 1);
                seq_t div2 = (seq_t)(2 * k + 2);

                binom = binom * mul1;
                binom = binom * mul2;
                binom = binom / div1;
                binom = binom / div2;
            }
        }

        seq_t result;
        if (base_neg) {
            /* a(cur) = -base_raw + sum_neg - sum_pos
               Compute via: result = sum_neg - (sum_pos + base_raw) */
            result = sum_neg;
            result -= sum_pos + base_raw;
        } else {
            /* a(cur) = base_raw + sum_neg - sum_pos */
            result = base_raw + sum_neg;
            result -= sum_pos;
        }

        a_cache[cur] = result;
    }

    a_cache_len = n + 1;
    return a_cache[n];
}

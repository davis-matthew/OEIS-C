/*
 * A393182  G.f. A(x) satisfies: A(x) = 1 + x^2 + x^3 * A(x)^2 / (1 - x).
 * https://oeis.org/A393182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 1; For n ≥ 3: a(n) = Σ_{j=0}^{n-3}
 *            Σ_{i=0}^{j} a(i) * a(j-i)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_arr   = NULL;   /* a(n) values                  */
static seq_t *conv_arr = NULL;  /* conv[j] = Σ_{i=0}^{j} a[i]*a[j-i] */
static seq_t *s_arr   = NULL;   /* prefix sums of conv           */
static idx_t  cache_size = 0;   /* number of computed elements   */
static size_t capacity   = 0;   /* allocated array size          */

/* ensure capacity for at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (capacity < needed) {
        size_t new_cap = capacity ? capacity * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        a_arr    = (seq_t *)realloc(a_arr,    new_cap * sizeof(seq_t));
        conv_arr = (seq_t *)realloc(conv_arr, new_cap * sizeof(seq_t));
        s_arr    = (seq_t *)realloc(s_arr,    new_cap * sizeof(seq_t));
        capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* compute new terms up to n */
    for (idx_t idx = cache_size; idx <= n; ++idx) {
        if (idx == 0) {
            a_arr[0] = (seq_t)1;
            conv_arr[0] = a_arr[0] * a_arr[0];
            s_arr[0] = conv_arr[0];
        } else if (idx == 1) {
            a_arr[1] = (seq_t)0;
            conv_arr[1] = a_arr[0] * a_arr[1] + a_arr[1] * a_arr[0];
            s_arr[1] = s_arr[0] + conv_arr[1];
        } else if (idx == 2) {
            a_arr[2] = (seq_t)1;
            conv_arr[2] = a_arr[0] * a_arr[2] + a_arr[1] * a_arr[1] + a_arr[2] * a_arr[0];
            s_arr[2] = s_arr[1] + conv_arr[2];
        } else {   /* idx >= 3 */
            a_arr[(size_t)idx] = s_arr[(size_t)(idx - 3)];

            /* compute conv[idx] = Σ_{i=0}^{idx} a[i] * a[idx-i] */
            seq_t sum = 0;
            for (idx_t i = 0; i <= idx; ++i) {
                sum += a_arr[(size_t)i] * a_arr[(size_t)(idx - i)];
            }
            conv_arr[(size_t)idx] = sum;

            /* update prefix sum s[idx] = s[idx-1] + conv[idx] */
            s_arr[(size_t)idx] = s_arr[(size_t)(idx - 1)] + conv_arr[(size_t)idx];
        }
    }

    cache_size = n + 1;
    return a_arr[(size_t)n];
}

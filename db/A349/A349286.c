/*
 * A349286  G.f. A(x) satisfies: A(x) = 1 / (1 - x * A(3*x)^2).
 * https://oeis.org/A349286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{i=0}^{n-1} Σ_{j=0}^{n-i-1} 3^(i+j) * a(i) *
 *            a(j) * a(n-i-j-1) (OEIS A349286)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *a_arr = NULL;      /* a(n) */
static seq_t *conv_arr = NULL;   /* conv(k) = Σ_{i=0}^{k} a(i)*a(k-i)   */
static seq_t *pow3_arr = NULL;   /* pow3(k) = 3^k */
static idx_t max_n = 0;          /* largest n computed */
static idx_t pow3_max = 0;       /* largest k for which pow3_arr[k] is ready */
static idx_t capacity = 0;       /* allocated capacity for each array */

/* Ensure arrays can hold indices up to 'needed' */
static void ensure_capacity(idx_t needed)
{
    if (capacity <= needed)
    {
        idx_t newcap = capacity ? (capacity << 1) : 1;
        while (newcap <= needed)
            newcap <<= 1;

        a_arr    = (seq_t *)realloc(a_arr,    newcap * sizeof(seq_t));
        conv_arr = (seq_t *)realloc(conv_arr, newcap * sizeof(seq_t));
        pow3_arr = (seq_t *)realloc(pow3_arr, newcap * sizeof(seq_t));

        capacity = newcap;
    }
}

/* One‑time initialisation of the base case */
static void init_sequence(void)
{
    if (capacity == 0)
    {
        ensure_capacity(0);

        a_arr[0]    = (seq_t)1;   /* a(0) = 1 */
        conv_arr[0] = (seq_t)1;   /* 1*1 */
        pow3_arr[0] = (seq_t)1;   /* 3^0 */

        max_n   = 0;
        pow3_max = 0;
    }
}

seq_t a(idx_t n)
{
    init_sequence();

    /* Compute all terms up to n if needed */
    while (max_n < n)
    {
        idx_t next = max_n + 1;              /* index to compute */

        ensure_capacity(next);               /* make room for next */

        /* Make sure powers of 3 up to next-1 are ready */
        if (pow3_max < next - 1)
        {
            idx_t i = pow3_max + 1;
            seq_t base = pow3_arr[pow3_max];
            for (; i <= next - 1; ++i)
            {
                base *= (seq_t)3;
                pow3_arr[i] = base;
            }
            pow3_max = next - 1;
        }

        /* a[next] = Σ_{s=0}^{next-1} 3^s * conv[s] * a[next-1-s] */
        seq_t sum = (seq_t)0;
        for (idx_t s = 0; s <= next - 1; ++s)
        {
            seq_t tmp = conv_arr[s];
            tmp *= pow3_arr[s];
            tmp *= a_arr[(next - 1) - s];
            sum += tmp;
        }
        a_arr[next] = sum;

        /* conv[next] = Σ_{i=0}^{next} a[i] * a[next-i] */
        seq_t conv_val = (seq_t)0;
        for (idx_t i = 0; i <= next; ++i)
            conv_val += a_arr[i] * a_arr[next - i];
        conv_arr[next] = conv_val;

        max_n = next;
    }

    return a_arr[n];
}

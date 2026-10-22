/*
 * A380406  E.g.f. satisfies A(x) = exp( 2 * x * exp(x) * A(x)^(1/2) ).
 * https://oeis.org/A380406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1..n} k^(n-k) * (k+2)^(k-1) * binomial(n,k);
 *            a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* exponentiation by squaring for big integers (works for built‑in and arbitrary‑precision types) */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ensure that the array can hold at least `needed` elements, fill new ones with 0 */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap > 0) ? *cap : 1;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = *cap; i < newcap; ++i) tmp[i] = 0;
    *arr = tmp;
    *cap = newcap;
}

/* static state for memoisation */
static idx_t  current_n = (idx_t)(-1);
static seq_t *a_arr     = NULL;  /* a[0..current_n]       */
static seq_t *pow_arr   = NULL;  /* k^(current_n-k)       */
static seq_t *binom_arr = NULL;  /* C(current_n,k)        */
static seq_t *base_arr  = NULL;  /* (k+2)^(k-1)           */
static size_t a_cap     = 0;
static size_t pow_cap   = 0;
static size_t binom_cap = 0;
static size_t base_cap  = 0;

/* initialise on first call */
static void init(void)
{
    if (current_n != (idx_t)(-1)) return;
    current_n = 0;
    ensure_cap(&a_arr,     &a_cap,     1);
    ensure_cap(&pow_arr,   &pow_cap,   1);
    ensure_cap(&binom_arr, &binom_cap, 1);
    ensure_cap(&base_arr,  &base_cap,  1);
    a_arr[0]     = 1;   /* a(0) */
    pow_arr[0]   = 1;   /* unused but fine */
    binom_arr[0] = 1;   /* C(0,0) */
    base_arr[0]  = 1;   /* unused */
}

/* sequence function requested by the build */
seq_t a(idx_t n)
{
    init();
    if (n == 0) return a_arr[0];

    /* extend memoisation to n if needed */
    if (current_n < n)
    {
        for (idx_t cur = current_n + 1; cur <= n; ++cur)
        {
            size_t need = (size_t)cur + 1;
            ensure_cap(&a_arr,     &a_cap,     need);
            ensure_cap(&pow_arr,   &pow_cap,   need);
            ensure_cap(&binom_arr, &binom_cap, need);
            ensure_cap(&base_arr,  &base_cap,  need);

            /* new term k = cur */
            pow_arr[cur] = 1; /* exponent 0 */
            base_arr[cur] = pow_uint((seq_t)(cur + 2), cur - 1);

            /* update existing powers: exponent grows by 1 */
            for (idx_t k = 1; k < cur; ++k)
                pow_arr[k] *= (seq_t)k;

            /* update binomial coefficients (C(cur,k)) */
            for (idx_t k = cur; k > 0; --k)
                binom_arr[k] = binom_arr[k] + binom_arr[k - 1];

            /* compute the sum for this n */
            seq_t sum = 0;
            for (idx_t k = 1; k <= cur; ++k)
            {
                seq_t term = pow_arr[k];
                term *= base_arr[k];
                term *= binom_arr[k];
                sum += term;
            }
            a_arr[cur] = sum * 2;
            current_n = cur;
        }
    }
    return a_arr[n];
}

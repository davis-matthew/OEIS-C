/*
 * A324030  Digits of one of the two 5-adic integers sqrt(-6) that is related
 *          to A324028.
 * https://oeis.org/A324030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A324028(n+1) - A324028(n)) / 5^n (digits of the 5‑adic
 *            square‑root of –6 ending with 3)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static state: digits of the square‑root and intermediate sum
   array for x² + 6.  All indices are in 0‑based base‑5 digit order.  */
static uint8_t *digits      = NULL;
static size_t   d_cap       = 0;
static size_t   d_len       = 0;

static int *sum             = NULL;
static size_t  s_cap        = 0;

/* Ensure the digits array can hold at least `need` elements. */
static void ensure_d_cap(size_t need)
{
    if (need > d_cap)
    {
        size_t new_cap = d_cap ? d_cap * 2 : 64;
        while (new_cap <= need) new_cap <<= 1;
        uint8_t *new_arr = (uint8_t *)realloc(digits, new_cap);
        if (!new_arr) abort();
        for (size_t i = d_cap; i < new_cap; ++i) new_arr[i] = 0;
        digits = new_arr;
        d_cap  = new_cap;
    }
}

/* Ensure the sum array can hold at least `need` elements. */
static void ensure_s_cap(size_t need)
{
    if (need > s_cap)
    {
        size_t new_cap = s_cap ? s_cap * 2 : 64;
        while (new_cap <= need) new_cap <<= 1;
        int *new_arr = (int *)realloc(sum, new_cap * sizeof(int));
        if (!new_arr) abort();
        for (size_t i = s_cap; i < new_cap; ++i) new_arr[i] = 0;
        sum = new_arr;
        s_cap = new_cap;
    }
}

/* Propagate carries so that all coefficients in `sum` are in 0..4. */
static void normalize_sum(void)
{
    size_t i;
    for (i = 0; i + 1 < s_cap; ++i)
    {
        int carry = sum[i] / 5;
        if (carry)
        {
            sum[i] -= carry * 5;
            sum[i + 1] += carry;
        }
    }
    /* Handle any remaining carry beyond the current capacity. */
    i = s_cap - 1;
    while (sum[i] >= 5)
    {
        int carry = sum[i] / 5;
        sum[i] -= carry * 5;
        ++i;
        ensure_s_cap(i + 1);
        sum[i] += carry;
    }
}

/* Initialise the first digit (3) and the corresponding sum array. */
static void init_sequence(void)
{
    ensure_d_cap(1);
    digits[0] = 3;
    d_len = 1;

    /* For x = 3: x² + 6 = 15 = 0·5⁰ + 3·5¹.  */
    ensure_s_cap(2);
    sum[0] = 15 % 5;        /* 0 */
    sum[1] = (15 / 5) % 5;  /* 3 */
    sum[2] = 0;
    normalize_sum();
}

/* Return the n‑th digit (0‑based) of the 5‑adic square‑root of –6. */
seq_t a(idx_t n)
{
    if (!digits) init_sequence();

    size_t target = (size_t)n;
    for (size_t k = d_len; k <= target; ++k)
    {
        /* Coefficient of 5^k in x_k² + 6. */
        int b = (k < s_cap) ? sum[k] : 0;

        /* Since 2·x ≡ 1 (mod 5) for all x ≡ 3 (mod 5),
           the digit d satisfies 2·x·d ≡ –b (mod 5), hence d = –b (mod 5). */
        uint8_t d = (uint8_t)((5 - (b % 5)) % 5);
        digits[k] = d;

        /* Update the sum array to reflect (x_k + d·5^k)² + 6. */
        for (size_t i = 0; i < k; ++i)
        {
            int add = 2 * (int)d * (int)digits[i];
            size_t pos = i + k;
            ensure_s_cap(pos + 1);
            sum[pos] += add;
        }
        size_t pos2 = 2 * k;
        ensure_s_cap(pos2 + 1);
        sum[pos2] += d * d;

        normalize_sum();
        d_len = k + 1;
    }

    return (seq_t)digits[target];
}

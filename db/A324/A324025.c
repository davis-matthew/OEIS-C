/*
 * A324025  Digits of one of the two 5-adic integers sqrt(6) that is related
 *          to A324023.
 * https://oeis.org/A324025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((sqrt(6) with leading digit 1 in base 5))_digit(n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Derived recurrence: a(0)=1, for n>0 */
/*      a(n) ≡ –3 * ((s_{n-1}^2 – 6)/5^n)  (mod 5)  where s_{n-1}=∑_{i=0}^{n-1}a(i)5^i */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Local state                                                    */
/* ------------------------------------------------------------------ */
static uint8_t *a_arr = NULL;    /* digits a(i)                     */
static size_t   a_sz  = 0;       /* allocated size for a_arr        */
static uint8_t *sq_arr = NULL;  /* digits of (∑a_i5^i)^2           */
static size_t   sq_sz  = 0;     /* allocated size for sq_arr       */
static idx_t    max_computed = 0;/* highest n whose digit is known  */

/* ------------------------------------------------------------------ */
/*  Helpers to grow the dynamic arrays                              */
/* ------------------------------------------------------------------ */
static void ensure_a_capacity(idx_t needed)
{
    if (a_sz < (size_t)needed) {
        size_t new_sz = a_sz ? a_sz * 2 : 64;
        while (new_sz < (size_t)needed) new_sz <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(a_arr, new_sz);
        if (!tmp) abort();
        if (new_sz > a_sz)
            memset(tmp + a_sz, 0, new_sz - a_sz);
        a_arr = tmp;
        a_sz = new_sz;
    }
}

static void ensure_sq_capacity(size_t needed)
{
    if (sq_sz < needed) {
        size_t new_sz = sq_sz ? sq_sz * 2 : 128;
        while (new_sz < needed) new_sz <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(sq_arr, new_sz);
        if (!tmp) abort();
        if (new_sz > sq_sz)
            memset(tmp + sq_sz, 0, new_sz - sq_sz);
        sq_arr = tmp;
        sq_sz = new_sz;
    }
}

/* ------------------------------------------------------------------ */
/*  Compute next digit and update the square representation           */
/* ------------------------------------------------------------------ */
static void add_digit(idx_t k)          /* compute a(k)   */
{
    /* ----- 1. Determine the coefficient D = ((s_{k-1}^2 – 6)/5^k) mod 5  ----- */
    ensure_sq_capacity(k + 1);          /* need sq[k] */
    uint8_t borrow = 0;
    int val;
    /* i=0 : subtract 6 from lowest digit */
    val = (int)sq_arr[0] - 6 - borrow;

    if (val < 0) { val += 5; borrow = 1; }
    else           borrow = 0;

    /* i=1 .. k-1 : propagate borrow */
    for (idx_t i = 1; i < k; ++i) {
        val = (int)sq_arr[i] - borrow;
        if (val < 0) { val += 5; borrow = 1; }
        else         borrow = 0;
    }

    /* D from the k-th digit after subtracting 6 */
    int D = (int)sq_arr[k] - borrow;
    if (D < 0) D += 5;

    /* ----- 2. Compute a(k)  ( 2*a_k ≡ -D (mod 5) ,  a_k ≡ -3*D (mod 5) ) ----- */
    int a_k = (5 - (3 * D) % 5) % 5;

    /* store digit */
    ensure_a_capacity(k + 1);
    a_arr[k] = (uint8_t)a_k;

    /* ----- 3. Update sq_arr to reflect (s_{k})^2  ------------------- */
    /* 3a. Add 2*s_{k-1}*a_k*5^k  terms : for j=0..k-1  */
    for (idx_t j = 0; j < k; ++j) {
        size_t idx = j + k;                    /* position in sq_arr */
        if (idx >= sq_sz) ensure_sq_capacity(idx + 1);
        uint32_t tmp = (uint32_t)sq_arr[idx] + 2 * a_k * a_arr[j];
        sq_arr[idx] = (uint8_t)(tmp % 5);
        uint32_t carry = tmp / 5;
        size_t pos = idx + 1;
        while (carry > 0) {
            if (pos >= sq_sz) ensure_sq_capacity(pos + 1);
            uint32_t t = (uint32_t)sq_arr[pos] + carry;
            sq_arr[pos] = (uint8_t)(t % 5);
            carry = t / 5;
            ++pos;
        }
    }

    /* 3b. Add a_k^2 * 5^(2k)  (only affects sq[2k] and higher)   */
    size_t idx2 = 2 * k;
    if (idx2 >= sq_sz) ensure_sq_capacity(idx2 + 1);
    uint32_t tmp2 = (uint32_t)sq_arr[idx2] + a_k * a_k;
    sq_arr[idx2] = (uint8_t)(tmp2 % 5);
    uint32_t carry2 = tmp2 / 5;
    size_t pos2 = idx2 + 1;
    while (carry2 > 0) {
        if (pos2 >= sq_sz) ensure_sq_capacity(pos2 + 1);
        uint32_t t = (uint32_t)sq_arr[pos2] + carry2;
        sq_arr[pos2] = (uint8_t)(t % 5);
        carry2 = t / 5;
        ++pos2;
    }
}

/* ------------------------------------------------------------------ */
/*  The public function as required by the exercise                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* first call: initialise the sequence */
    if (max_computed == 0 && n == 0) {
        ensure_a_capacity(1);
        a_arr[0] = 1;          /* a(0) = 1 */
        ensure_sq_capacity(2);
        sq_arr[0] = 1;         /* (a0)^2 = 1 */
        sq_arr[1] = 0;
        max_computed = 0;
        return (seq_t)1;
    }

    if (max_computed == 0) {   /* initialise if not yet done */
        ensure_a_capacity(1);
        a_arr[0] = 1;
        ensure_sq_capacity(2);
        sq_arr[0] = 1;
        sq_arr[1] = 0;
    }

    /* compute up to required n */
    for (idx_t k = max_computed + 1; k <= n; ++k)
        add_digit(k);

    max_computed = n;
    return (seq_t)a_arr[n];
}

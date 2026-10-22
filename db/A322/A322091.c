/*
 * A322091  Digits of one of the two 13-adic integers sqrt(-3).
 * https://oeis.org/A322091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n‑th base‑13 digit of the 13‑adic square root of –3 that
 *            ends with digit 6. Computation via Hensel lifting: the algorithm
 *            keeps the operand root modulo 13^k and the base‑13
 *            representation of root²+3, updating digit by digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *digits = NULL;          /* digits of the root (base 13)           */
static size_t   digits_cap = 0;          /* capacity of digits array             */
static int *prod = NULL;                 /* base‑13 representation of (root²+3)   */
static size_t   prod_cap = 0;             /* capacity of prod array               */
static size_t   computed = 0;             /* number of digits already known       */

static void ensure_digits_cap(size_t needed)
{
    if (needed > digits_cap) {
        size_t new_cap = digits_cap ? digits_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        uint8_t *new_ptr = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        if (!new_ptr) abort();
        digits = new_ptr;
        digits_cap = new_cap;
    }
}

static void ensure_prod_cap(size_t needed)
{
    if (needed > prod_cap) {
        size_t new_cap = prod_cap ? prod_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        int *new_ptr = (int *)realloc(prod, new_cap * sizeof(int));
        if (!new_ptr) abort();

        /* Zero‑initialize any newly allocated cells */
        if (new_cap > prod_cap) {
            size_t old = prod_cap;
            prod_cap = new_cap;
            prod = new_ptr;
            for (size_t i = old; i < prod_cap; ++i) prod[i] = 0;
        } else {
            prod = new_ptr;
        }
    }
}

static void propagate_from(size_t pos)
{
    size_t i = pos;
    while (1) {
        int v = prod[i];
        prod[i] = v % 13;
        int carry = v / 13;
        if (!carry) break;
        ++i;
        ensure_prod_cap(i + 1);
        prod[i] += carry;
    }
}

seq_t a(idx_t n)
{
    size_t need_digits = (size_t)n + 1;
    size_t need_prod   = (size_t)2 * n + 1;
    ensure_digits_cap(need_digits);
    ensure_prod_cap(need_prod);

    if (computed == 0) {
        digits[0] = 6;          /* a(0) = 6 */
        prod[0]   = 39;         /* 6² + 3   */
        propagate_from(0);
        computed = 1;
    }

    for (size_t m = computed; m <= (size_t)n; ++m) {
        int a_m = prod[m];
        digits[m] = (uint8_t)a_m;

        for (size_t i = 0; i < m; ++i) {
            prod[i + m] += 2 * a_m * digits[i];
        }
        prod[2 * m] += a_m * a_m;
        propagate_from(m);
    }

    computed = (size_t)n + 1;
    return (seq_t)digits[(size_t)n];
}

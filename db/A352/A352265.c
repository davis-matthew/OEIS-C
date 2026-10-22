/*
 * A352265  Integers that need 7 iterations of the map x->A352172(x) to reach
 *          1.
 * https://oeis.org/A352265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for numbers that require exactly
   7 iterations of the map f(x) = (product of non‑zero decimal digits
   of x)^3 to reach 1.  The sequence is generated on demand
   and cached for subsequent calls.  Big integer arithmetic is
   implemented in base‑10 digits to provide exact results for all
   starting values that fit into seq_t.  The implementation works
   for C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Big integer implementation (base‑10) ---------- */
#define MAX_DIGITS 2000   /* enough for up to ~1600 decimal digits */

typedef struct {
    uint8_t digits[MAX_DIGITS];   /* least significant digit first */
    size_t len;                   /* number of used digits */
} BigInt;

/* Initialise BigInt from an unsigned 64‑bit value */
static void big_init(BigInt *a, uint64_t v) {
    if (v == 0) {
        a->len = 1;
        a->digits[0] = 0;
        return;
    }
    a->len = 0;
    while (v) {
        a->digits[a->len++] = (uint8_t)(v % 10);
        v /= 10;
    }
}

/* Set BigInt to 1 */
static void big_set_one(BigInt *a) {
    a->len = 1;
    a->digits[0] = 1;
}

/* Test whether BigInt equals 1 */
static bool big_is_one(const BigInt *a) {
    return a->len == 1 && a->digits[0] == 1;
}

/* Multiply BigInt * d (d between 1 and 9), in place */
static void big_mul_small(BigInt *a, uint8_t d) {
    if (d == 1) return;
    uint32_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint32_t prod = a->digits[i] * d + carry;
        a->digits[i] = (uint8_t)(prod % 10);
        carry = prod / 10;
    }
    while (carry) {
        a->digits[a->len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

/* Multiply BigInt a * b, store result in res */
static void big_mul(BigInt *res, const BigInt *a, const BigInt *b) {
    memset(res->digits, 0, MAX_DIGITS);
    res->len = a->len + b->len;
    for (size_t i = 0; i < a->len; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < b->len; ++j) {
            uint32_t sum = res->digits[i + j] + a->digits[i] * b->digits[j] + carry;
            res->digits[i + j] = (uint8_t)(sum % 10);
            carry = sum / 10;
        }
        size_t k = i + b->len;
        while (carry) {
            uint32_t sum = res->digits[k] + carry;
            res->digits[k] = (uint8_t)(sum % 10);
            carry = sum / 10;
            ++k;
        }
    }
    /* Trim possible leading zeros */
    while (res->len > 1 && res->digits[res->len - 1] == 0) {
        --res->len;
    }
}

/* Compute BigInt x^3, store in res */
static void big_cube(BigInt *res, const BigInt *x) {
    BigInt sq;
    big_mul(&sq, x, x);     /* square */
    big_mul(res, &sq, x);   /* cube */
}

/* Product of non‑zero decimal digits of BigInt x */
static void big_product_of_digits(BigInt *product, const BigInt *x) {
    big_set_one(product);
    for (size_t i = 0; i < x->len; ++i) {
        uint8_t d = x->digits[i];
        if (d != 0) {
            big_mul_small(product, d);
        }
    }
}

/* ---------- Sequence logic ---------- */

/* Return true if starting value val reaches 1 in exactly 7 iterations. */
static bool reaches_in_seven_steps(seq_t val) {
    BigInt x;
    big_init(&x, (uint64_t)val);

    for (int step = 1; step <= 7; ++step) {
        BigInt prod;
        big_product_of_digits(&prod, &x);   /* product of non‑zero digits */

        BigInt next;
        big_cube(&next, &prod);             /* cube of that product */

        if (big_is_one(&next)) {
            return step == 7;
        }
        x = next; /* move to next iterate */
    }
    return false;
}

seq_t a(idx_t n) {
    /* Static cache of found sequence terms. */
    static seq_t *values = NULL;
    static size_t values_len = 0;
    static size_t values_cap = 0;
    static idx_t tested_max = 0;   /* largest number already tested */

    /* Extend the cache until it contains at least n terms. */
    while (values_len < n) {
        ++tested_max;                              /* candidate number */

        if (reaches_in_seven_steps((seq_t)tested_max)) {
            /* Grow the result array if needed */
            if (values_len == values_cap) {
                size_t new_cap = values_cap ? values_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(values,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failed */
                values = tmp;
                values_cap = new_cap;
            }
            values[values_len++] = (seq_t)tested_max; /* store the term */
        }
    }

    return values[n - 1];   /* return the requested term (1‑based index) */
}

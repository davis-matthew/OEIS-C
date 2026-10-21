/*
 * A259469  Digits of an idempotent 12-adic number.
 * https://oeis.org/A259469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=9. For n>0,
   let t = coefficient of 12ⁿ in the convolution of the already‑known digits of e.
   Then 5·a(n) ≡ –t (mod 12).  This algorithm builds the digits of the 12‑adic
   idempotent e (e² = e) iteratively, storing the convolution for fast
   updates. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for digits and convolution of e² */
    static uint8_t *digits = NULL;           /* digits of e, 0 … 11 */
    static unsigned *prod   = NULL;          /* convolution coefficients, base 12 */
    static size_t digits_cap = 0;
    static size_t prod_cap   = 0;
    static size_t prod_size  = 0;            /* highest index potentially non‑zero */
    static size_t num_digits = 0;            /* number of digits already computed */

    /* Grow digits array if necessary */
    if ((size_t)n + 1 > digits_cap) {
        size_t new_cap = (size_t)n + 1;
        new_cap = new_cap * 2;
        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof *tmp);
        if (!tmp) abort();
        if (digits_cap < new_cap)
            memset(tmp + digits_cap, 0,
                   (new_cap - digits_cap) * sizeof *tmp);
        digits = tmp;
        digits_cap = new_cap;
    }

    /* Grow prod array (size up to 2*(n+1)+10 for safety) */
    size_t new_prod_cap = 2 * ((size_t)n + 1) + 10;
    if (new_prod_cap > prod_cap) {
        unsigned *tmp = (unsigned *)realloc(prod, new_prod_cap * sizeof *tmp);
        if (!tmp) abort();
        if (prod_cap < new_prod_cap)
            memset(tmp + prod_cap, 0,
                   (new_prod_cap - prod_cap) * sizeof *tmp);
        prod = tmp;
        prod_cap = new_prod_cap;
    }

    /* Initialise first digit if not yet done */
    if (num_digits == 0) {
        digits[0] = 9;
        /* prod[0] = 9*9 = 81, propagate carry to get base‑12 digits */
        prod[0] = 9u * 9u;
        size_t pos = 0;
        while (prod[pos] >= 12u) {
            unsigned carry = prod[pos] / 12u;
            prod[pos] %= 12u;
            prod[pos + 1] += carry;
            ++pos;
        }
        prod_size = pos;
        num_digits = 1;
    }

    /* Compute digits up to index n */
    for (size_t k = num_digits; k <= (size_t)n; ++k) {
        /* coefficient of 12^k in e² after carry propagation */
        unsigned t = prod[k] % 12u;

        /* Solve 5*d ≡ –t (mod 12)  →  d = (12 – (5*t)%12)%12 */
        unsigned d = (12u - (5u * t) % 12u) % 12u;
        digits[k] = (uint8_t)d;

        /* Update convolution prod with new digit d */
        for (size_t i = 0; i < k; ++i) {
            prod[i + k] += 2u * d * digits[i];
        }
        prod[2 * k] += d * d;
        if (2 * k > prod_size) prod_size = 2 * k;

        /* Propagate carries from position k upwards */
        for (size_t pos = k; pos <= prod_size; ++pos) {
            unsigned carry = prod[pos] / 12u;
            prod[pos] %= 12u;
            if (carry) {
                prod[pos + 1] += carry;
                if (pos == prod_size) prod_size = pos + 1;
            }
        }
    }

    num_digits = (size_t)n + 1;
    return (seq_t)digits[(size_t)n];
}

/*
 * A358991  a(n) is the number of zero digits in the product of the first n
 *          odd numbers not divisible by 5.
 * https://oeis.org/A358991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of zero digits in the product of the first n odd *
 *            numbers not divisible by 5. The product is built incrementally;
 *            at each * step we multiply the current product by the next odd
 *            number that is not a * multiple of 5 and count the zero digits
 *            in the decimal representation
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Digits array (little‑endian decimal digits)                            */
/* --------------------------------------------------------------------- */
static uint8_t *digits          = NULL;   /* decimal digits          */
static size_t  digits_len       = 0;      /* number of used digits   */
static size_t  digits_cap       = 0;      /* allocated capacity      */

/* multiply current decimal number (digits array) by an unsigned integer
 * factor (up to a few tens of thousands).  The result is written back
 * into the same array, growing it if necessary.                       */
static void mult_by_uint(uint64_t factor)
{
    uint64_t carry = 0;
    size_t i;

    for (i = 0; i < digits_len; ++i) {
        uint64_t prod = (uint64_t)digits[i] * factor + carry;
        digits[i]    = (uint8_t)(prod % 10);
        carry        = prod / 10;
    }
    while (carry) {
        if (digits_len == digits_cap) {
            size_t new_cap = digits_cap ? digits_cap << 1 : 64;
            uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
            if (!tmp) abort();
            digits = tmp;
            digits_cap = new_cap;
        }
        digits[digits_len++] = (uint8_t)(carry % 10);
        carry                /= 10;
    }
}

/* --------------------------------------------------------------------- */
/*  Sequence cache                                                          */
/* --------------------------------------------------------------------- */
static seq_t *a_cache        = NULL;    /* a(0),a(1),…              */
static size_t a_len          = 0;       /* number of stored terms    */
static size_t a_cap          = 0;       /* allocated array capacity  */
static idx_t  max_n          = 0;       /* largest n computed        */

/* next odd number not divisible by 5 (global state)                     */
static idx_t cur_odd = 1;                /* starts at 1 */

static void grow_a_cache(size_t new_cap)
{
    if (new_cap <= a_cap) return;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cap   = new_cap;
}

static void init_state(void)
{
    if (a_cap) return;   /* already initialised */

    /* initialise digits array with value 1 (product for n = 0) */
    digits_cap = 64;
    digits     = (uint8_t *)malloc(digits_cap);
    if (!digits) abort();
    digits[0] = 1;
    digits_len = 1;

    a_cap   = 16;                     /* small start */
    a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    if (!a_cache) abort();
    a_cache[0] = (seq_t)0;            /* a(0) = 0 */
    a_len    = 1;
    cur_odd   = 1;                    /* first odd not a multiple of 5 */
    max_n    = 0;
}

static void ensure_upto(idx_t n)
{
    init_state();
    if (n <= max_n) return;

    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* multiply the product by the current odd number */
        mult_by_uint((uint64_t)cur_odd);

        /* count zero digits in the current product */
        unsigned zeros = 0;
        for (size_t j = 0; j < digits_len; ++j)
            if (digits[j] == 0)
                ++zeros;

        /* store the result into the cache */
        if (a_len >= a_cap)
            grow_a_cache(a_cap ? (a_cap << 1) : 16);
        a_cache[a_len++] = (seq_t)zeros;

        /* advance cur_odd to the next odd number not divisible by 5 */
        do {
            cur_odd += 2;
        } while ((cur_odd % 5) == 0);
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    ensure_upto(n);
    return a_cache[n];
}

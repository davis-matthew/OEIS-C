/*
 * A368008  Decimal expansion of 171/93, being the highest possible points
 *          won/lost ratio in a completed 5-set tennis match which the player
 *          loses.
 * https://oeis.org/A368008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*
 * Formula: a(n) is the n‑th decimal digit (1‑based) of the number 171/93,
 * starting with the integer part as the first digit.
 */

/* static state for cached digits */
static seq_t *digits = NULL;
static idx_t digits_len = 0;   /* number of digits stored (>=1 after init) */
static size_t digits_cap = 0;  /* allocated capacity of digits */
static seq_t remainder = 0;    /* remainder after integer part */

/* rational constants */
static const seq_t NUMERATOR   = (seq_t)171;
static const seq_t DENOMINATOR = (seq_t)93;

/* ensure the digits array has space for at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (digits_cap >= (size_t)needed) return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 128;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* allocation failed */
    digits = tmp;
    digits_cap = new_cap;
}

/* sequence entry a(n): n-th decimal digit (1‑based) of 171/93 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1; safe default */

    /* initialize on first use */
    if (digits_len == 0) {
        ensure_capacity(1);
        digits[0] = NUMERATOR / DENOMINATOR;  /* integer part (1) */
        remainder = NUMERATOR % DENOMINATOR;  /* start remainder */
        digits_len = 1;
    }

    /* generate digits up to the requested index if needed */
    if (n > digits_len) {
        ensure_capacity(n);
        for (idx_t i = digits_len; i < n; ++i) {
            seq_t prod = remainder * (seq_t)10;
            digits[i] = prod / DENOMINATOR;    /* next decimal digit */
            remainder = prod % DENOMINATOR;    /* update remainder */
        }
        digits_len = n;
    }

    return digits[n - 1];           /* 1‑based index */
}

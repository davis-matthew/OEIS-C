/*
 * A360180  Decimal expansion of the electron volt-hertz relationship
 *          according to the 2019 SI system in units Hz.
 * https://oeis.org/A360180
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  cons, easy, nonn
 * Formula:   a(n) = nth decimal digit of 241798924208491.81622382914252726...
 *            offset = 15 (a(15) is the first digit)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define INT_START    ((idx_t)15)
#define INT_LENGTH   ((idx_t)15)
#define INT_END      ((idx_t)(INT_START + INT_LENGTH - 1))

/* cache for fractional digits */
static seq_t *dig_arr   = NULL;
static size_t dig_cap   = 0;
static size_t dig_cnt   = 0;

/* current remainder in the long division for the fractional part */
static seq_t rem = 36055709;   /* remainder of 36055709 / 44173801 */

/* powers of 10 for extracting integer–part digits */
static seq_t pow10[INT_LENGTH];

/* initialization flag */
static bool initialized = false;

/* ------------------------------------------------------------ */
static void init(void)
{
    if (initialized) return;

    /* pre‑compute powers of ten up to 10^14 */
    pow10[0] = 1;
    for (idx_t i = 1; i < INT_LENGTH; ++i)
        pow10[i] = pow10[i - 1] * 10;

    /* allocate initial capacity for fractional digits */
    dig_cap = 128;
    dig_arr = (seq_t *)malloc(dig_cap * sizeof(seq_t));
    dig_cnt = 0;
    initialized = true;
}

static void ensure_capacity(size_t need)
{
    if (need <= dig_cap) return;
    size_t new_cap = dig_cap ? dig_cap : 128;
    while (new_cap < need) new_cap <<= 1;
    dig_arr = (seq_t *)realloc(dig_arr, new_cap * sizeof(seq_t));
    dig_cap = new_cap;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < INT_START) return 0;   /* undefined before the offset */

    init();

    /* integer part digits (positions 15 .. 29) */
    if (n <= INT_END) {
        idx_t k   = n - INT_START;            /* 0‑based index within the 15 digits */
        idx_t exp = INT_LENGTH - 1 - k;       /* power of ten to divide by        */
        seq_t integer_part = (seq_t)241798924208491ULL;
        seq_t digit = (integer_part / pow10[exp]) % 10;
        return digit;
    }

    /* fractional part: position after the decimal point  */
    idx_t idx = n - INT_END - 1;  /* 0‑based index into fractional digits */

    if (idx >= dig_cnt) {
        ensure_capacity(idx + 1);
        while (dig_cnt <= idx) {
            seq_t digit = (rem * 10) / 44173801;
            rem        = (rem * 10) % 44173801;
            dig_arr[dig_cnt++] = digit;
        }
    }
    return dig_arr[idx];
}

/*
 * A271223  Digits of one of the two 3-adic integers sqrt(-2).
 * https://oeis.org/A271223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A271223 – Digits of one of the two 3‑adic integers sqrt(-2).
 *
 *  The digits a(n) are obtained by a simple 3‑adic Hensel lifting.
 *  For each k ≥ 1 we keep the unordered convolution array
 *
 *          raw[d] = Σ_{i+j=d} a(i)·a(j) ,
 *
 *  where the sum runs over all existing digits.  The value
 *
 *          t = x_k² + 2 = Σ raw[d]·3^d + 2
 *
 *  is then carried in base 3.  The digit at position k of that
 *  representation is precisely a(k).  Updating `raw` for the new
 *  digit is O(k), so the whole computation up to n is O(n²),
 *  easily fast enough for the required limits.
 *
 *  The function is memoised; any order of calls is handled correctly.
 *
 *  Note: `seq_t` may be a fixed‑size integer or an arbitrary‑precision
 *  integer type.  The algorithm uses only integers of at most
 *  32 bits for the convolution array; these operations are well
 *  defined for all candidate types.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Memoised storage of the sequence                                            */
/* ------------------------------------------------------------------ */

static uint8_t  *digits = NULL;   /* digits a(i)  (0,1,2)                */
static size_t   digits_len = 0;   /* number of digits computed          */
static size_t   digits_cap = 0;   /* allocated capacity for digits      */
static unsigned *rawProd = NULL;  /* convolution coefficients raw[d]   */
static size_t   raw_cap   = 0;    /* allocated capacity for rawProd    */

/* ------------------------------------------------------------------ */
/*  Ensure that at least (n+1) digits and enough rawProd entries exist   */
/* ------------------------------------------------------------------ */

static void ensure_capacity(idx_t n)
{
    size_t need       = (size_t)n + 1;           /* number of digits   */
    size_t raw_need   = 2 * need + 5;           /* max index used in rawProd */

    /* first initialisation */
    if (digits == NULL) {
        digits_cap = 16;
        digits = (uint8_t*)malloc(digits_cap * sizeof(uint8_t));
        digits[0] = 1;                 /* a(0) = 1                          */
        digits_len = 1;

        raw_cap = 32;
        rawProd = (unsigned*)malloc(raw_cap * sizeof(unsigned));
        for (size_t i = 0; i < raw_cap; ++i) rawProd[i] = 0;
        rawProd[0] = 1;                /* raw[0] = a0*a0 = 1                 */
    }

    /* grow digits array if required */
    if (digits_cap < need) {
        size_t new_cap = digits_cap;
        while (new_cap < need) new_cap <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(digits, new_cap * sizeof(uint8_t));
        digits = tmp;
        digits_cap = new_cap;
    }

    /* grow rawProd array if required */
    if (raw_cap < raw_need) {
        size_t new_cap = raw_cap ? raw_cap : 32;
        while (new_cap < raw_need) new_cap <<= 1;
        unsigned *tmp = (unsigned*)realloc(rawProd, new_cap * sizeof(unsigned));
        /* initialise new entries to zero */
        for (size_t i = raw_cap; i < new_cap; ++i) tmp[i] = 0;
        rawProd = tmp;
        raw_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Compute digits up to index `n` (inclusive)                          */
/* ------------------------------------------------------------------ */

static void compute_up_to(idx_t n)
{
    ensure_capacity(n);

    while (digits_len <= (size_t)n) {
        size_t k = digits_len;             /* index to compute */

        /* carry propagation for t = x_k² + 2 */
        unsigned carry   = 0;
        unsigned digit_k = 0;

        for (unsigned m = 0; m <= k; ++m) {
            unsigned val = rawProd[m];
            if (m == 0) val += 2;          /* +2 at 3⁰ */
            val += carry;
            unsigned dig = val % 3;
            carry = val / 3;
            if (m == k) digit_k = dig;    /* digit at position k */
        }

        uint8_t a_k = (uint8_t)digit_k;
        digits[k] = a_k;
        digits_len = k + 1;

        /* update rawProd for the new digit */
        for (size_t i = 0; i < k; ++i) {
            rawProd[i + k] += 2 * (unsigned)digits[i] * (unsigned)a_k;
        }
        rawProd[2 * k] += (unsigned)a_k * (unsigned)a_k;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the test harness                        */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    compute_up_to(n);
    return (seq_t)digits[(size_t)n];
}

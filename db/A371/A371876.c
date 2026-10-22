/*
 * A371876  Decimal expansion of 2^3021377 - 1, the 37th Mersenne prime.
 * https://oeis.org/A371876
 *
 * Offset:    909526    (a(909526) is the first term)
 * Keywords:  nonn, cons, fini
 * Formula:   a(n) is the (n-909526+1)-th decimal digit of 2^3021377-1
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   get_digits, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/get_digits.h"

static uint8_t *digit_cache = NULL;
static size_t digit_count  = 0;
static const idx_t EXPONENT = 3021377ULL;    /* 2^3021377 - 1 */
static const idx_t OFFSET   = 909526ULL;     /* a(909526) is the first digit */

static void init_digits(void) {
    /* Compute the Mersenne prime 2^EXPONENT - 1 */
    seq_t prime = pow2(EXPONENT);
    prime = prime - 1;   /* 2^EXPONENT - 1 */

    /* First pass: get the number of decimal digits */
    size_t len = 0;
    get_digits(prime, NULL, &len);

    /* Allocate buffer for the digits (most‑significant first) */
    uint8_t *buf = (uint8_t*)malloc(len * sizeof(uint8_t));
    if (!buf)
        return;  /* allocation failed; leave cache empty */

    /* Second pass: retrieve the digits into the buffer */
    get_digits(prime, buf, &len);

    /* Utilities deliver digits least‑significant first.  Reverse to
       obtain the canonical decimal order. */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = tmp;
    }

    digit_cache  = buf;
    digit_count  = len;
}

seq_t a(idx_t n) {
    if (n < OFFSET)
        return 0;   /* Undefined below the offset */

    if (!digit_cache)
        init_digits();   /* Lazy initialization */

    if (!digit_cache)
        return 0;   /* Failed to initialise */

    size_t idx = (size_t)(n - OFFSET);
    if (idx >= digit_count)
        return 0;   /* n exceeds number of digits */

    return (seq_t)digit_cache[idx];
}

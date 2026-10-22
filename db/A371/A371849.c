/*
 * A371849  Decimal expansion of 2^132049 - 1, the 30th Mersenne prime.
 * https://oeis.org/A371849
 *
 * Offset:    39751    (a(39751) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = digit at position (n-39750) of the decimal expansion of
 *            2^132049 - 1 (the 30th Mersenne prime)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow2, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/seq_to_str.h"

static const idx_t OFFSET = 39751;           /* start index of the sequence */
static char *dec_digits = NULL;             /* buffer holding the decimal digits */
static size_t dec_len = 0;                  /* number of digits stored */
static int initialized = 0;                 /* flag to avoid recomputation */

static void init_sequence(void)
{
    if (initialized)
        return;
    initialized = 1;

    /* Compute 2^132049 - 1 as a big integer */
    seq_t m = pow2(132049);
    m -= (seq_t)1;

    /* Allocate buffer big enough for all digits + null terminator */
    /* The 30th Mersenne prime has 39751 decimal digits, so 40000 bytes is safe */
    size_t buf_size = 40000;
    char *buf = (char *)malloc(buf_size);
    if (!buf)                          /* allocation failure – abort */
        return;

    size_t out_len;
    seq_to_str(m, buf, &out_len);      /* convert to decimal string */
    dec_digits = buf;
    dec_len = strlen(buf);              /* guaranteed to be 39751 */
}

seq_t a(idx_t n)
{
    if (n < OFFSET)                     /* indices below the offset are undefined */
        return (seq_t)0;

    init_sequence();

    size_t idx = (size_t)(n - OFFSET);  /* 0‑based index into dec_digits */
    if (idx >= dec_len)
        return (seq_t)0;                /* out of range – return 0 */

    return (seq_t)(dec_digits[idx] - '0');
}

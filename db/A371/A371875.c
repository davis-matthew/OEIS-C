/*
 * A371875  Decimal expansion of 2^2976221 - 1, the 36th Mersenne prime.
 * https://oeis.org/A371875
 *
 * Offset:    895932    (a(895932) is the first term)
 * Keywords:  nonn, cons, fini
 * Formula:   a(n) = decimal digit at position n of the decimal expansion of
 *            2^2976221 − 1 (Mersenne prime). For n ≥ 895932, a(n) is the n‑th
 *            decimal digit of that prime, with a(895932) = 6, a(895933) = 2,
 *            …
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   big_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper routines from the utils library:
   - big_pow(seq_t,int): computes a^e
   - seq_to_str(seq_t,char*,size_t*): converts a big integer to a
     null‑terminated decimal string, filling the length if buf==NULL */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/seq_to_str.h"

static const idx_t OFFSET = 895932;   /* first digit index */

/* Cached decimal representation of 2^2976221 − 1 */
static char *digits = NULL;
static size_t digits_len = 0;

/* Initialise the cached decimal string once. */
static void init(void)
{
    const seq_t exponent = (seq_t)2976221;
    seq_t prime = big_pow((seq_t)2, exponent);  /* prime = 2^2976221   */
    prime -= (seq_t)1;                          /* prime = 2^2976221-1 */

    /* Determine required buffer length. */
    size_t len = 0;
    seq_to_str(prime, NULL, &len);

    /* Allocate and store the decimal string. */
    digits = (char *)malloc(len + 1);
    seq_to_str(prime, digits, &len);
    digits[len] = '\0';
    digits_len = len;
}

seq_t a(idx_t n)
{
    if (n < OFFSET)                     /* behaviour below offset is unspecified */
        return (seq_t)0;

    if (digits == NULL)                 /* first call – compute once */
        init();

    idx_t pos = n - OFFSET;             /* 0‑based position in the string */
    if (pos >= digits_len)              /* safety guard */
        return (seq_t)0;

    return (seq_t)(digits[pos] - '0');  /* digit 0 – 9 fits in any seq_t */
}

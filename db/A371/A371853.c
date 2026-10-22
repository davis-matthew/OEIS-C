/*
 * A371853  Decimal expansion of 2^1257787 - 1, the 34th Mersenne prime.
 * https://oeis.org/A371853
 *
 * Offset:    378632    (a(378632) is the first term)
 * Keywords:  nonn, cons, fini
 * Formula:   return the decimal digits of 2^1257787 – 1, indexed from 378632.
 *            The first call precomputes the prime as a decimal string using
 *            the arbitrary–precision helpers pow2 and seq_to_str
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow2, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_to_str.h"

static char *prime_str = NULL;
static size_t prime_len = 0;
static const idx_t OFFSET = 378632ULL;
static int prime_ready = 0;

/* Compute 2^1257787 and store its decimal representation (minus 1) */
static void init_prime(void)
{
    if (prime_ready)
        return;

    /* 2^1257787 as a big integer. */
    seq_t p = pow2((idx_t)1257787);

    /* First find length, then allocate buffer. */
    size_t len;
    seq_to_str(p, NULL, &len);

    prime_len = len;
    prime_str = (char *)malloc(len + 1);
    seq_to_str(p, prime_str, &len);

    /* Subtract 1: the last digit is guaranteed to be > '0'. */
    prime_str[len - 1]--;

    prime_str[len] = '\0';
    prime_ready = 1;
}

seq_t a(idx_t n)
{
    if (n < OFFSET)
        return (seq_t)0;

    init_prime();

    idx_t pos = n - OFFSET;
    if ((size_t)pos >= prime_len)
        return (seq_t)0;

    return (seq_t)(prime_str[pos] - '0');
}

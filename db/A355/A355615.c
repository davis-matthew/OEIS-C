/*
 * A355615  Define a rational sequence {b(n)} as b(1) = 1, b(n) = b(n-1) +
 *          1/(n + 1 - b(n-1)) for n > 1; a(n) is the numerator of b(n).
 * https://oeis.org/A355615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   b(1)=1; b(n)=b(n-1)+1/(n+1-b(n-1)); a(n) = numerator of b(n)
 *            (indices start at 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *num = NULL;    /* numerators of b(k) */
static seq_t *den = NULL;    /* denominators of b(k) */
static idx_t  cap   = 0;     /* current capacity of arrays  */
static idx_t  max_n = 0;     /* highest n computed so far */

/* Ensure capacity for indices up to n (inclusive).  */
static void ensure_capacity(idx_t n)
{
    if (n <= cap)
        return;
    idx_t new_cap = n * 2;
    seq_t *new_num = (seq_t *)realloc(num, (new_cap + 1) * sizeof(seq_t));
    seq_t *new_den = (seq_t *)realloc(den, (new_cap + 1) * sizeof(seq_t));
    if (!new_num || !new_den)
        exit(EXIT_FAILURE);
    num = new_num;
    den = new_den;
    cap = new_cap;
}

/* Main function: returns a(n). */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined; return 0 as a safe default */
        return (seq_t)0;

    /* Lazy initialization of storage if needed. */
    if (num == NULL) {
        cap = 2;                /* capacity for indices 0..2 */
        num = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        den = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!num || !den)
            exit(EXIT_FAILURE);
        num[1] = (seq_t)1;      /* b(1) = 1/1 */
        den[1] = (seq_t)1;
        max_n  = 1;
    }

    if (n <= max_n)
        return num[n];

    /* Allocate space for required indices. */
    ensure_capacity(n);

    /* Compute b(k) for k = max_n+1 … n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t p = num[k-1];            /* numerator of b(k-1) */
        seq_t q = den[k-1];            /* denominator of b(k-1) */

        seq_t mul1_minus_p = (seq_t)(k + 1) * q - p;   /* (k+1)q - p */

        /* New numerator and denominator of b(k). */
        seq_t new_num = p * mul1_minus_p + q * q;     /* p*D + q*q   */
        seq_t new_den = q * mul1_minus_p;             /* q*D         */

        seq_t g = gcd(new_num, new_den);             /* reduce fraction */
        num[k] = new_num / g;
        den[k] = new_den / g;
    }

    max_n = n;
    return num[n];
}

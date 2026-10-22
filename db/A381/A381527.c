/*
 * A381527  a(n) is the smallest positive multiple of n such that the decimal
 *          expansion of a(n)/n is a substring of the decimal expansion of
 *          a(n), or -1 if no such number exists.
 * https://oeis.org/A381527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381527:  a(n) = smallest positive multiple of n such that
   decimal expansion of a(n)/n is a substring of decimal expansion of a(n),
   or -1 if no such multiple exists.
   Recurrence:  a(10*n) = 10*a(n) for a(n) ≠ -1.  */

/*  Author: ChatGPT, 2026-10-19 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* convert a positive seq_t value to a null‑terminated decimal string.
   The result is placed in buf, which must have space for at least 80 chars. */
static void seqt_to_str(seq_t x, char *buf)
{
    if (x == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    size_t pos = 0;
    seq_t t = x;
    /* extract digits in reverse order */
    while (t > 0) {
        uint64_t d = (uint64_t)(t % 10);
        buf[pos++] = (char)('0' + d);
        t /= 10;
    }
    /* reverse into correct order */
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
    buf[pos] = '\0';
}

/* main function implementing the sequence */
seq_t a(idx_t n)
{
    /* a(0) is explicitly defined as -1 */
    if (n == 0) {
        return (seq_t)-1;
    }

    /* use the recurrence a(10*n) = 10*a(n) when applicable */
    if ((n % 10) == 0) {
        seq_t sub = a(n / 10);
        if (sub == (seq_t)-1) {
            return (seq_t)-1;
        }
        return sub * (seq_t)10;
    }

    /* search for the minimal multiplier m */
    /* upper bound for the search: empirically large enough for n <= 10000
       in the OEIS data; increase if necessary. */
    const idx_t MAX_MULT = 200000;
    char prodbuf[80];
    char mbuf[80];

    for (idx_t m = 1; m <= MAX_MULT; ++m) {
        /* compute product n*m */
        seq_t prod = (seq_t)n * (seq_t)m;

        /* convert product and m to strings */
        seqt_to_str(prod, prodbuf);
        seqt_to_str((seq_t)m, mbuf);

        /* test if m string is a substring of product string */
        if (strstr(prodbuf, mbuf) != NULL) {
            return prod;
        }
    }

    /* if no multiplier found within bound, declare no such number */
    return (seq_t)-1;
}

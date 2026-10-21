/*
 * A260273  Successively add the smallest nonzero binary number that is not a
 *          substring.
 * https://oeis.org/A260273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 9071, u32 n <= 9297, u64 n <= 9297,
 *            u128 n <= 9297, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260273:  a(1)=1; for n≥1,
   a(n+1) = a(n) + A261461(a(n)),
   where A261461(x) is the smallest positive integer whose binary
   representation is not a substring of the binary representation of x. */

#include <stdbool.h>
#include <string.h>

/* Return binary length of v (>=1). */
static unsigned bitlen_seq_t(seq_t v)
{
    unsigned len = 0;
    seq_t tmp = v;
    while (tmp != (seq_t)0) {
        ++len;
        tmp >>= 1;
    }
    return len;
}

/* Convert v to binary string in buf; buf must have space for at least
   bitlen_seq_t(v)+1 bytes. Non‑null terminated string is written. */
static void to_bitstring(seq_t v, char *buf, size_t bufsize)
{
    unsigned len = bitlen_seq_t(v);
    if (len == 0) {
        /* Should not happen for positive numbers, but keep safe. */
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    if (len >= bufsize) len = (unsigned)(bufsize - 1);        /* safety */
    unsigned i;
    for (i = 0; i < len; ++i)
        buf[len - 1 - i] = ((v >> i) & (seq_t)1) ? '1' : '0';
    buf[len] = '\0';
}

/* Return true iff pat is a substring of x (both given as binary strings). */
static bool contains_substring(const char *xstr, const char *patstr)
{
    return strstr(xstr, patstr) != NULL;
}

/* Smallest positive integer not appearing as a binary substring of x. */
static seq_t smallest_missing(seq_t x)
{
    static char bitsx[2048];
    static char bitsp[256];

    to_bitstring(x, bitsx, sizeof(bitsx));

    seq_t k = (seq_t)1;
    while (true) {
        to_bitstring(k, bitsp, sizeof(bitsp));
        if (!contains_substring(bitsx, bitsp))
            return k;
        ++k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)              /* offset starts at 1, behavior for 0 is undefined */
        return (seq_t)0;

    seq_t prev = (seq_t)1;   /* a(1) */
    if (n == 1)
        return prev;

    /* Sequentially compute up to a(n). */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t add = smallest_missing(prev);
        prev = prev + add;
    }
    return prev;
}

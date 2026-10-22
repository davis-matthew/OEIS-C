/*
 * A361973  Decimal expansion of twice the Champernowne constant.
 * https://oeis.org/A361973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, cons
 * Formula:   a(n) = (2 * d_n + (d_{n+1} >= 5 ? 1 : 0)) mod 10 where d_i is
 *            the i‑th digit of Champernowne's constant C = 0.12345678910...
 *            The digit a(n) of 2·C depends only on the current digit d_n and
 *            the next digit d_{n+1}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of Champernowne digits */
    static uint8_t *Cdigits = NULL;
    static size_t  Clen   = 0;   /* number of digits currently stored      */
    static size_t  Ccap   = 0;   /* allocated capacity for Cdigits          */
    static uint64_t nextnum = 1; /* next integer to append to the sequence */

    /* We need digits up to index n+1 (to use d[n] and d[n+1]) */
    size_t required = (size_t)n + 2;

    /* Grow storage if necessary */
    if (required > Clen) {
        if (Ccap < required) {
            size_t newcap = Ccap ? Ccap : 1;
            while (newcap < required) newcap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(Cdigits, newcap);
            Cdigits = tmp;
            Ccap    = newcap;
        }
        /* Generate Champernowne digits until we have at least 'required' of them */
        while (Clen < required) {
            uint64_t x = nextnum;
            unsigned char buf[32];
            size_t pos = 0;
            do {
                buf[pos++] = (unsigned char)(x % 10);
                x /= 10;
            } while (x);
            /* Append the digits in normal order */
            for (size_t i = pos; i > 0; --i)
                Cdigits[Clen++] = buf[i-1];
            ++nextnum;
        }
    }

    /* Compute a(n) using the simple rule derived above */
    seq_t cur    = Cdigits[(size_t)n];
    seq_t nxt    = Cdigits[(size_t)(n + 1)];
    seq_t carry = (nxt >= 5) ? 1 : 0;
    return ((seq_t)2 * cur + carry) % 10;
}

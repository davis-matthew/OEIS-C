/*
 * A273397  a(n) = Fibonacci(Catalan(n)).
 * https://oeis.org/A273397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(Catalan(n))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 9 terms through n = 8 (2026-11-01)
 * Helpers:   bit_length, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bit_length.h"
#include "../utils/catalan.h"

static seq_t fib_big(seq_t n)
{
    /* Fast–doubling Fibonacci for an arbitrary–precision index n. */
    if (n == 0) return 0;

    unsigned bits = bit_length(n);
    seq_t a = 0, b = 1;           /* (F(k), F(k+1)) for the current prefix */

    /* Process bits from most significant to least significant. */
    for (unsigned i = bits; i-- > 0; )
    {
        /* c = F(2k),   d = F(2k+1) based on current (a,b) = (F(k),F(k+1)) */
        seq_t two_b_minus_a = (b << 1) - a;
        seq_t c = a * two_b_minus_a;
        seq_t d = a * a + b * b;

        /* Extract the i‑th bit of the index n. */
        unsigned bit = ((n >> i) & 1) ? 1 : 0;

        if (bit == 0)
        {
            a = c;
            b = d;
        }
        else
        {
            a = d;
            b = c + d;
        }
    }

    return a;    /* F(n) */
}

seq_t a(idx_t n)
{
    /* Catalan(n) is the index, possibly enormous. */
    seq_t cat = catalan(n);

    /* Fibonacci of that huge index.  The result fits exactly into seq_t
       whenever it fits in the target type (or the arbitrary‑precision build
       will produce the full value). */
    return fib_big(cat);
}

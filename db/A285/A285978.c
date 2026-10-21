/*
 * A285978  {00->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transform: start with the Thue–Morse sequence t(k)=parity(k).
   Output each t(k).  Whenever t(k)=t(k+1)=0, output a single 1
   (i.e., skip the second 0).  The function returns the n-th output
   bit (1‑based).  t(k) is 0 for even binary weight, 1 for odd. */

#include <stdint.h>

/* parity of a 64‑bit integer: 1 if odd number of 1‑bits, 0 otherwise */
static inline uint8_t parity64(idx_t x)
{
    uint64_t y = (uint64_t)x;
    y ^= y >> 32;
    y ^= y >> 16;
    y ^= y >>  8;
    y ^= y >>  4;
    y ^= y >>  2;
    y ^= y >>  1;
    return (uint8_t)(y & 1);
}

seq_t a(idx_t n)
{
    idx_t count = 0;   /* number of output bits already produced */
    idx_t k = 0;       /* current index in the Thue–Morse sequence */

    while (1) {
        uint8_t tk   = parity64(k);
        uint8_t tnext = parity64(k + 1);

        if (tk == 0 && tnext == 0) {
            /* 00 pair: output a single 1 and skip the second 0 */
            ++count;
            if (count == n)
                return (seq_t)1;
            k += 2;
        } else {
            /* single bit */
            ++count;
            if (count == n)
                return (seq_t)tk;
            k += 1;
        }
    }
}

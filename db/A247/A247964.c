/*
 * A247964  Beatty sequence for e^(1/3): a(n) = floor(n*(e^(1/3))).
 * https://oeis.org/A247964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(e^(1/3)))
 * Range:     u8 n <= 183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a high‑precision rational approximation of e^(1/3):
          e^(1/3) ≈ 1.395612425086089000.
       Multiplying by 10¹⁸ gives the integer 1395612425086089000.
       Writing
            1395612425086089000 = 1·10¹⁸ + 395612425086089000
       we obtain
            floor(n*e^(1/3)) = n + floor(n·395612425086089000 / 10¹⁸).
       The 10¹⁸ denominator and the remainder 395612425086089000 both
       fit in 64 bits, and the product n·395612425086089000 fits in
       128 bits for all 0 ≤ n < 2⁶⁴.  */
    static const unsigned long long DEN = 1000000000000000000ULL;   /* 10¹⁸ */
    static const unsigned long long R   = 395612425086089000ULL;    /* remainder */

    /* Compute the adjustment term using 128‑bit arithmetic to avoid
       overflow. */
    unsigned __int128 prod = (unsigned __int128)n * R;
    seq_t add = (seq_t)(prod / DEN);

    return (seq_t)n + add;
}

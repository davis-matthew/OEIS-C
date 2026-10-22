/*
 * A380572  Complement of A380509.
 * https://oeis.org/A380572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer x such that 4*x+1 is prime OR
 *            (4*x+1)/3 is prime (when 4*x+1 is divisible by 3)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Deterministic trial division for 64‑bit integers.  */
static bool isPrime64(uint64_t v)
{
    if (v < 2) return false;
    if (v == 2 || v == 3) return true;
    if ((v & 1U) == 0 || v % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= v; i += 6) {
        if (v % i == 0 || v % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of valid terms found so far   */
    for (idx_t i = 1; ; ++i) { /* iterate over positive integers       */
        seq_t x = (seq_t)i;            /* candidate value to return         */
        uint64_t p = 4ULL * (uint64_t)i + 1; /* compute 4*i+1 safely in 64‑bit */

        bool good = false;
        if (isPrime64(p))                /* 4*i+1 is prime              */
            good = true;
        else if (p % 3 == 0) {
            uint64_t q = p / 3;           /* (4*i+1)/3                    */
            if (isPrime64(q))             /* ...and that quotient is prime*/
                good = true;
        }

        if (good) {                      /* x satisfies the property     */
            ++count;
            if (count == n) return x;     /* nth valid term found          */
        }
    }
}

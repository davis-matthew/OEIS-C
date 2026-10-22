/*
 * A351835  Smallest odious number k (member of A000069) such that k*n is also
 *          odious.
 * https://oeis.org/A351835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest odious number k (odious with odd bit‑count) such that k*n is odious; k is always odd. */
#include <stdbool.h>

static bool is_odious(seq_t x)
{
    unsigned parity = 0;
    while (x)
    {
        parity ^= (unsigned)(x & 1);
        x >>= 1;
    }
    return parity == 1;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t k = 1;
    while (1)
    {
        if (is_odious(k) && is_odious(k * N))
            return k;
        k += 2;  /* keep k odd */
    }
}

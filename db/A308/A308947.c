/*
 * A308947  a(n) = A000129(A214028(n)+1) mod n.
 * https://oeis.org/A308947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000129(A214028(n)+1) mod n * Computation: let M =
 *            [[2,1],[1,0]]. * Iterate powers of M modulo n until the
 *            off–diagonal entries become 0. * Then a(n) is the scalar
 *            multiple r (the top‑left entry)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus for all computations */
    seq_t mod = (seq_t)n;

    /* Initial matrix M^1 = [[2,1],[1,0]] modulo n */
    seq_t x = 2 % mod;   /* top‑left   */
    seq_t y = 1 % mod;   /* top‑right  */
    seq_t z = 1 % mod;   /* bottom‑left*/
    seq_t w = 0 % mod;   /* bottom‑right */

    for (;;)
    {
        /* Stop when the matrix is a scalar multiple of the identity */
        if (y == 0 && z == 0)
            return x % mod;

        /* Compute the next power: (current) * M */
        seq_t nx = (x + x + y) % mod;   /* 2*x + y   */
        seq_t ny = x;                    /* x         */
        seq_t nz = (z + z + w) % mod;   /* 2*z + w   */
        seq_t nw = z;                    /* z         */

        x = nx;
        y = ny;
        z = nz;
        w = nw;
    }
}

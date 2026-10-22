/*
 * A340669  Permutation of the nonnegative integers formed by negation in
 *          complex base i-1.
 * https://oeis.org/A340669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = obtain the complex integer represented by n in base
 *            (i‑1), negate it, and re‑encode it to an integer. The algorithm
 *            works for any non‑negative idx_t n and returns seq_t a(n). For n
 *            up to 10000 the intermediate values fit easily in signed 64‑bit
 *            integers; for larger n the same algorithm sti...
 * Range:     u8 n <= 15, u16 n <= 4095, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow_imag_next, pow_real_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>
#include "../utils/pow_imag_next.h"
#include "../utils/pow_real_next.h"

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Step 1: compute z(n) = Σ bit_k * (i-1)^k                             */
    int64_t real   = 0;
    int64_t imag   = 0;
    int64_t powR   = 1;  /* (i-1)^0 */
    int64_t powI   = 0;

    idx_t mask = 1;
    while (mask <= n) {
        if (n & mask) {
            real += powR;
            imag += powI;
        }
        /* prepare next power (i-1)^(k+1) */
        int64_t nextR = pow_real_next(powR, powI);
        int64_t nextI = pow_imag_next(powR, powI);
        powR = nextR;
        powI = nextI;
        mask <<= 1;
    }

    /* Step 2: negate */
    real = -real;
    imag = -imag;

    /* Step 3: encode back to integer by repeated division by (i-1) */
    seq_t result = 0;
    unsigned bit_index = 0;

    while (real != 0 || imag != 0) {
        /* r = (real + imag) mod 2  (parity of the sum) */
        int r = ((real + imag) & 1) ? 1 : 0;

        /* subtract r from real part (imag remains unchanged) */
        int64_t x = real - r;
        int64_t y = imag;

        /* compute quotient: (x + yi) / (-1 + i)  */
        /*  (x + yi) * (-1 - i) / 2  */
        __int128 qR128 = ((__int128)(-x) + y) / 2;   /* (-x + y) / 2 */
        __int128 qI128 = -((__int128)(x + y) / 2);   /* -(x + y) / 2 */

        int64_t qR = (int64_t)qR128;
        int64_t qI = (int64_t)qI128;

        if (r) {
            result |= ((seq_t)1 << bit_index);
        }
        ++bit_index;
        real = qR;
        imag = qI;
    }

    return result;
}

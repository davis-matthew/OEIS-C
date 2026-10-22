/*
 * A309650  a(1) = 3, a(2) = 1, a(3) = 4, a(4) = 2; a(n) = a(n-a(n-2)) +
 *          a(n-a(n-3)) for n > 4.
 * https://oeis.org/A309650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5k) = 5, a(5k+1) = 5*floor(sqrt(k)+½) – 2, a(5k+2) = 5k + 1,
 *            a(5k+3) = 5k + 4, a(5k+4) = 5*floor(sqrt(k)) + 2 (k ≥ 1);
 *            a(1)=3, a(2)=1, a(3)=4, a(4)=2. This implements the closed form
 *            supplied by OEIS for A309650
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for idx_t n.
   Uses only integer arithmetic and stays within bounds for the
   n values encountered in this table. */
static seq_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    /* Avoid overflow: stop when (r+1)² > n. */
    while (r + 1 <= n / (r + 1))
        ++r;
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    /* Handle the initial four terms explicitly. */
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)3;
            case 2: return (seq_t)1;
            case 3: return (seq_t)4;
            case 4: return (seq_t)2;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* n ≥ 5: use the modulo‑5 formula. */
    idx_t r = n % 5;
    idx_t k = 0;          /* will be set for each branch */
    switch (r) {
        case 0:                     /* n = 5k */
            return (seq_t)5;

        case 1: {                   /* n = 5k+1, k ≥ 1 */
            k = (n - 1) / 5;
            seq_t s = isqrt_u64(k);                 /* floor(sqrt(k)) */
            /* round(sqrt(k)) = s + 1 if k ≥ s² + s + 1 */
            seq_t round_s = s + ((seq_t)k >= s * s + s + 1 ? 1 : 0);
            return 5 * round_s - 2;
        }

        case 2:                     /* n = 5k+2 */
            k = (n - 2) / 5;
            return 5 * k + 1;

        case 3:                     /* n = 5k+3 */
            k = (n - 3) / 5;
            return 5 * k + 4;

        case 4: {                   /* n = 5k+4 */
            k = (n - 4) / 5;
            seq_t s = isqrt_u64(k);                 /* floor(sqrt(k)) */
            return 5 * s + 2;
        }

        default:
            return (seq_t)0; /* unreachable */
    }
}

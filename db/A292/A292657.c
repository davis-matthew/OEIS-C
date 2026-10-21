/*
 * A292657  Rank of n when all the numbers h, j*cos(1), k*sin(1), for h>=1,
 *          j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n / cos(1)⌋ + ⌊n / sin(1)⌋ approximated with
 *            rational 1/cos(1) ≈ 1850815717680925549/10^18 1/sin(1) ≈
 *            1188395104643089400/10^18 to compute the floors exactly for n up
 *            to at least 10,000
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use 128‑bit arithmetic for the intermediate product so that
   (n * numerator) never overflows. */
#include <stdint.h>

static const seq_t COS_INV_NUM = 1850815717680925549ULL; /* 1/cos(1) × 10^18 */
static const seq_t COS_INV_DEN = 1000000000000000000ULL; /* 10^18 */
static const seq_t SIN_INV_NUM = 1188395104643089400ULL; /* 1/sin(1) × 10^18 */
static const seq_t SIN_INV_DEN = 1000000000000000000ULL; /* 10^18 */

seq_t a(idx_t n)
{
    /* Convert n once to seq_t for consistency in the calculation. */
    seq_t nn = (seq_t)n;

    /* Integer part contributed by the integers h themselves. */
    seq_t h = nn;

    /* Use 128‑bit intermediate multiplication to avoid overflow. */
    seq_t c = (seq_t)(((__uint128_t)nn * COS_INV_NUM) / COS_INV_DEN);
    seq_t s = (seq_t)(((__uint128_t)nn * SIN_INV_NUM) / SIN_INV_DEN);

    /* The rank is the sum of all three contributions. */
    return h + c + s;
}

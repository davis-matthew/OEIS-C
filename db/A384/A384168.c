/*
 * A384168  a(n) = 3^n * n! * binomial(4*n/3,n) * Sum_{k=1..n} 1/(n+3*k).
 * https://oeis.org/A384168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Prod_{i=0}^{n-1}(4*n-3*i)/(n+3*k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product P = ∏_{i=0}^{n-1}(4*n-3*i) is divisible by every n+3*k,
   so a(n) = ∑_{k=1}^n P/(n+3*k).  Computing P directly overflows
   64‑bit integers for n>12.  When seq_t is an integral type (uint8_t,
   uint16_t, uint32_t, uint64_t) we compute each term by multiplying
   all factors except the one equal to the denominator, which stays
   within the bit‑width for all n whose values fit in seq_t.  For
   non‑integral types (128‑bit or arbitrary precision) we compute P
   once and divide by each denominator.  This works for all build
   configurations. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t four_n = 4 * n;
    seq_t sum = 0;

    /* If seq_t is an integral type (≤ 64 bits), compute each term
       individually to avoid overflow.  Otherwise, compute the full
       product once (works for 128‑bit or arbitrary precision). */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* Small integral type: n is small enough that each
           (n-1)-fold product fits in seq_t. */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t denom = (seq_t)(n + 3 * k);
            seq_t term = 1;
            for (idx_t i = 0; i < n; ++i) {
                seq_t factor = (seq_t)(four_n - 3 * i);
                if (factor == denom) continue;   /* skip the divisor */
                term *= factor;
            }
            sum += term;
        }
    } else {
        /* Non‑integral type: compute the full product once. */
        seq_t P = 1;
        for (idx_t i = 0; i < n; ++i) {
            seq_t factor = (seq_t)(four_n - 3 * i);
            P *= factor;
        }
        for (idx_t k = 1; k <= n; ++k) {
            seq_t denom = (seq_t)(n + 3 * k);
            sum += P / denom;
        }
    }

    return sum;
}

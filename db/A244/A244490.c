/*
 * A244490  Triangle read by rows: T(n,k) (0 <= k <= n) = Sum_{i=0..[k/2]}
 *          (-1)^i*binomial(k,2*i)*(2*i-1)!!*n^(k-2*i).
 * https://oeis.org/A244490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(n,k) = sum_{i=0..⌊k/2⌋} (-1)^i *
 *            binomial(k,2*i) * (2i-1)!! * n^(k-2*i)
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 64, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (1) {
        __uint128_t next = (__uint128_t)(r + 1) * (r + 2) / 2;
        if (next > n) break;
        r++;
    }
    /* Column k within the row */
    idx_t prev = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - prev;

    /* Compute T(r,k) */
    idx_t halfk = k / 2;
    seq_t sum = 0;
    seq_t oddfact = 1;          /* (2i-1)!! */
    for (idx_t i = 0; i <= halfk; ++i) {
        seq_t choose = binomial(k, 2 * i);
        seq_t pow_n  = pow_seq((seq_t)r, k - 2 * i);
        seq_t term   = choose;
        term = term * oddfact;
        term = term * pow_n;
        if ((i & 1ULL) == 0) {
            sum += term;
        } else {
            sum -= term;
        }
        if (i + 1 <= halfk) {
            /* Update odd factorial for next i: multiply by (2*(i+1)-1) */
            oddfact = oddfact * (seq_t)(2 * (i + 1) - 1);
        }
    }
    return sum;
}

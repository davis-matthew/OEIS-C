/*
 * A323409  Greatest common divisor of Product (p_i^e_i)-1 and n, when n =
 *          Product (p_i^e_i); a(n) = gcd(n, A047994(n)).
 * https://oeis.org/A323409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd( n , ∏_{p^e∥n} (p^e – 1) )
 * Range:     u8 n <= 1403, u16 n <= 9770, u32 n <= 9770, u64 n <= 9770,
 *            u128 n <= 9770, big any n
 * Helpers:   factor_u64, gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    size_t cnt;
    uint64_t pr[20], ex[20];
    factor_u64(n, pr, ex, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = pow_int((seq_t)pr[i], (idx_t)ex[i]) - (seq_t)1;
        prod = prod * term;
    }

    return gcd((seq_t)n, prod);
}

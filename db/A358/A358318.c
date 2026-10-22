/*
 * A358318  For n >= 5, a(n) is the number of zeros that need to be inserted
 *          to the left of the ones digit of the n-th prime so that the result
 *          is composite.
 * https://oeis.org/A358318
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is defined by a search: for the nth prime p, let base =
 *            p/10 and d = p%10. a(n) is the smallest k≥1 such that
 *            base*10^(k+1)+d is composite
 * Range:     u8 none, u16 n <= 19, u32 n <= 19, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n < 5) return 0;          /* offset is 5, behaviour below is irrelevant */

    uint32_t p = get_prime(n);    /* nth prime */
    uint32_t base_u = p / 10;     /* all digits except the ones digit */
    uint32_t d_u   = p % 10;      /* ones digit */

    seq_t base = (seq_t)base_u;
    seq_t d    = (seq_t)d_u;

    for (idx_t k = 1;; ++k) {
        seq_t num = base * pow10(k + 1) + d;
        if (!is_prime_big(num)) {
            return (seq_t)k;
        }
    }
}

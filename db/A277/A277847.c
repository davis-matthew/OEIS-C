/*
 * A277847  Size of the largest subset of Z/nZ fixed by x -> x^2.
 * https://oeis.org/A277847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p==2 ? 2 : odd_part(p-1)*p^(e-1) + 1)
 * Range:     u8 n <= 522, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_part, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/odd_part.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    idx_t temp = n;
    seq_t result = 1;

    /* handle factor 2 */
    if (temp % 2 == 0) {
        idx_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        /* a(2^e) = 2 for any e >= 1 */
        result *= 2;
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t odd = odd_part((seq_t)(p - 1));
            seq_t power = pow_int((seq_t)p, exp - 1);
            seq_t contrib = odd * power + 1;
            result *= contrib;
        }
    }

    /* remaining prime > 1 */
    if (temp > 1) {
        seq_t odd = odd_part((seq_t)(temp - 1));
        /* e = 1 -> exp = 0, so power = 1 */
        seq_t contrib = odd + 1;
        result *= contrib;
    }

    return result;
}

/*
 * A336375  Numbers k such that gcd(k, prime(k) + prime(k+2)) > 1.
 * https://oeis.org/A336375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that gcd(k, prime(k) + prime(k+2))
 *            > 1
 * Range:     u8 n <= 21, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, safe fallback */

    idx_t found = 0;
    idx_t k = 1;          /* start checking from k = 1 */

    while (1) {
        /* obtain required primes by their indices */
        uint32_t pk  = get_prime(k);
        uint32_t pk2 = get_prime(k + 2);

        /* compute gcd(k, pk + pk2) */
        seq_t sum = (seq_t)pk + (seq_t)pk2;
        seq_t g   = gcd((seq_t)k, sum);

        /* count only if the gcd is greater than 1 */
        if (g > (seq_t)1) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k; /* move to the next integer */
    }
}

/*
 * A379009  a(n) = 2*n^2 XOR 1+sigma(n^2).
 * https://oeis.org/A379009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^2) XOR (1 + sigma(n^2))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 4321, u64 n <= 9306,
 *            u128 n <= 9306, big any n
 * Helpers:   sigma_of_nsq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of_nsq.h"

seq_t a(idx_t n)
{
    /* compute 2*n^2 */
    idx_t nsq = n * n;
    seq_t two_nsq = ((seq_t)nsq) * 2;

    /* compute sigma(n^2) and add 1 */
    seq_t s_plus_one = sigma_of_nsq(n) + 1;

    /* XOR the two values */
    return two_nsq ^ s_plus_one;
}

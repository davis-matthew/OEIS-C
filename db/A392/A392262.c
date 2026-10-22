/*
 * A392262  Even composite numbers that are squarefree or powers of 2.
 * https://oeis.org/A392262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th even composite number that is either a power of
 *            two (≥4) or a squarefree multiple of 2 (i.e., 2·m with m odd,
 *            squarefree, and m>1). Equivalent to the disjoint union of
 *            A000079\{1,2} and A039956\{2}. The implementation simply scans
 *            integers, testing the above conditions
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_power_of_two.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, but return 0 for safety */

    idx_t cand   = 4;             /* first term is 4 */
    idx_t count  = 1;             /* number of terms found so far */

    /* Scan upward until the n‑th qualifying number is found. */
    while (count < n) {
        cand++;

        /* Skip odd numbers – only even numbers can qualify. */
        if ((cand & 1) != 0)
            continue;

        /* Powers of two (≥4) always qualify. */
        if (is_power_of_two(cand)) {
            count++;
            continue;
        }

        /* Otherwise, candidate must be 2·m where m is odd, squarefree,
           and m>1.  The condition cand % 4 == 2 ensures exactly one
           factor of 2.  The is_squarefree test checks that no prime
           factor repeats. */
        if ((cand & 3) == 2 && is_squarefree((seq_t)cand)) {
            count++;
        }
    }

    return (seq_t)cand;
}

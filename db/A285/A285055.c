/*
 * A285055  Sum of the cycle lengths of the elements 0,1,...n-1, in Zn under
 *          multiplication.
 * https://oeis.org/A285055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{a=0}^{n-1} cycle_length(a) where cycle_length(a) is
 *            the length of the eventual cycle reached by the iterates a ->
 *            a*a mod n -> a*a*a mod n -> ... (0 and 1 have cycle length 1)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t i;
    seq_t sum = 0;
    seq_t N = (seq_t)n;

    for (i = 0; i < n; ++i)
    {
        seq_t a_val = (seq_t)i;
        seq_t cycle_len;

        if (i == 0)            /* 0 always maps to 0 */
        {
            cycle_len = 1;
        }
        else
        {
            /* Floyd's cycle‑finding algorithm on the sequence x -> a_val * x mod N */
            seq_t tortoise = (a_val * a_val) % N;         /* f(a)   */
            seq_t hare     = (tortoise * a_val) % N;     /* f(f(a)) */
            hare = (hare * a_val) % N;                    /* double step */

            while (tortoise != hare)
            {
                tortoise = (tortoise * a_val) % N;
                hare     = (hare * a_val) % N;
                hare     = (hare * a_val) % N;
            }

            /* Find cycle length λ */
            cycle_len = 1;
            hare        = (tortoise * a_val) % N;
            while (hare != tortoise)
            {
                hare = (hare * a_val) % N;
                ++cycle_len;
            }
        }
        sum += cycle_len;
    }

    return sum;
}

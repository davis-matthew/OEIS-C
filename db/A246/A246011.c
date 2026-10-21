/*
 * A246011  a(n) = Product_{i in row n of A245562} Lucas(i+1), where Lucas =
 *          A000204.
 * https://oeis.org/A246011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{L∈runs_of_1s(binary(n))} Lucas(L+1)
 * Range:     u8 n <= 682, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Pre‑compute Lucas numbers for indices up to 65 (covers all run lengths for n ≤ 10 000). */
static seq_t lucas[66];
static bool lucas_initialized = false;

static void init_lucas(void)
{
    if (lucas_initialized) return;
    lucas[0] = (seq_t)2;
    lucas[1] = (seq_t)1;
    for (int i = 2; i < 66; ++i)
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    lucas_initialized = true;
}

/* Compute the n-th term of A246011. */
seq_t a(idx_t n)
{
    init_lucas();

    seq_t result = (seq_t)1;
    idx_t m = n;

    while (m != 0)
    {
        /* Skip trailing zeros – not part of a run. */
        while ((m & (idx_t)1) == 0)
            m >>= 1;

        /* Count consecutive ones. */
        unsigned int run = 0;
        while ((m & (idx_t)1) != 0)
        {
            ++run;
            m >>= 1;
        }

        /* Multiply by Lucas(run+1).  run+1 ≤ 65, safe index. */
        result *= lucas[run + 1];
    }

    return result;
}

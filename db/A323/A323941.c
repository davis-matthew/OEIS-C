/*
 * A323941  Total number of isomers (nonisomorphic systems) of unbranched
 *          tri-4-catafusenes as a function of the number of hexagons (see
 *          Cyvin et al. (1996) for precise definition).
 * https://oeis.org/A323941
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323941: Number of isomers of unbranched tri‑4‑catafusenes.
   Computed by the 12‑term linear recurrence derived from its rational
   generating function; initial values:
   a(3)=1, a(4)=3, a(5)=16, a(6)=62, a(7)=275, a(8)=1121, a(9)=4584,
   a(10)=18012, a(11)=69573, a(12)=262495, a(13)=974704.
   Recurrence for n>=14:
   a(n) = 16*a(n‑1) – 102*a(n‑2) + 304*a(n‑3) – 247*a(n‑4) –
          1056*a(n‑5) + 3372*a(n‑6) – 3168*a(n‑7) – 2223*a(n‑8) +
          8208*a(n‑9) – 8262*a(n‑10) + 3888*a(n‑11) – 729*a(n‑12). */

#include <stddef.h>

/* Initial values for indices 0..13 (0 for indices <3). */
static const uint64_t init_vals[14] = {
    0ULL, 0ULL, 0ULL,      /* a(0), a(1), a(2) – unused but keep symmetry */
    1ULL, 3ULL, 16ULL, 62ULL, 275ULL, 1121ULL,
    4584ULL, 18012ULL, 69573ULL, 262495ULL, 974704ULL
};

/* Public function. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* sequence undefined for n < 3 */

    if (n <= 13)                      /* return known exact values */
        return (seq_t)init_vals[n];

    /* Ring buffer holding the last 12 terms needed for the recurrence. */
    seq_t last[12];

    /* Initialise the buffer for n = 14:
       last[0] = a(2), last[1] = a(3), … , last[10] = a(12), last[11] = a(13). */
    last[0] = (seq_t)init_vals[2];
    last[1] = (seq_t)init_vals[3];
    last[2] = (seq_t)init_vals[4];
    last[3] = (seq_t)init_vals[5];
    last[4] = (seq_t)init_vals[6];
    last[5] = (seq_t)init_vals[7];
    last[6] = (seq_t)init_vals[8];
    last[7] = (seq_t)init_vals[9];
    last[8] = (seq_t)init_vals[10];
    last[9] = (seq_t)init_vals[11];
    last[10] = (seq_t)init_vals[12];
    last[11] = (seq_t)init_vals[13];

    /* Compute successive terms up to n. */
    for (idx_t i = 14; i <= n; ++i)
    {
        seq_t next =
            (seq_t)16   * last[11] -
            (seq_t)102  * last[10] +
            (seq_t)304  * last[9]  -
            (seq_t)247  * last[8]  -
            (seq_t)1056 * last[7]  +
            (seq_t)3372 * last[6]  -
            (seq_t)3168 * last[5]  -
            (seq_t)2223 * last[4]  +
            (seq_t)8208 * last[3]  -
            (seq_t)8262 * last[2]  +
            (seq_t)3888 * last[1]  -
            (seq_t)729  * last[0];

        /* Shift buffer left by one; drop oldest term. */
        for (size_t k = 0; k < 11; ++k)
            last[k] = last[k + 1];
        last[11] = next;
    }

    return last[11];                 /* last term holds a(n) */
}

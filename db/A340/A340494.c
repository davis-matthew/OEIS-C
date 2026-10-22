/*
 * A340494  Index where n first appears in A340488.
 * https://oeis.org/A340494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1 a(n) = a(n‑1) + g(v2(n)+1), where v2(n) is
 *            the exponent of the highest power of 2 dividing n, and g is
 *            defined by g(1)=2, g(2)=3, g(3)=8, g(4)=33, g(k) = 6·g(k‑1) –
 *            9·g(k‑2) + 4·g(k‑3) (k ≥ 5)
 * Range:     u8 n <= 23, u16 n <= 383, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include minimal standard headers needed for the types used. */
#include <stdint.h>

/* The function required by the build. */
seq_t a(idx_t n)
{
    /* Static storage for the sequence g(k).  Only indices 1..max_t are used. */
    static seq_t g[70];
    static idx_t g_len = 0;  /* highest index of g that has been computed */

    /* Ensure g has the base values. */
    if (g_len == 0) {
        g[0]  = 0;   /* unused */
        g[1]  = 2;
        g[2]  = 3;
        g[3]  = 8;
        g[4]  = 33;
        g_len = 4;
    }

    /* Base value a(0). */
    seq_t result = 1;

    /* Compute a(n) by iterating from 1 through n.  This is O(n·log n),
       which is well below the time limit for n up to ~10 000. */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute v2(i) – number of trailing zero bits. */
        idx_t t = 0;
        idx_t tmp = i;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++t;
        }

        /* Ensure g contains the value g(t+1). */
        if ((t + 1) > g_len) {
            for (idx_t j = g_len + 1; j <= t + 1; ++j) {
                /* Recurrence for k ≥ 5.  g[1]..g[4] are already set. */
                g[j] = 6 * g[j - 1] - 9 * g[j - 2] + 4 * g[j - 3];
            }
            g_len = t + 1;
        }

        /* Add the corresponding difference to the cumulative sum. */
        result += g[t + 1];
    }

    return result;
}

/*
 * A283900  Relative of Hofstadter Q-sequence: a(-769) = 770, a(-768) = 4,
 *          a(-767) = 770, a(-766) = 4; thereafter a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)).
 * https://oeis.org/A283900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) with a(-769) = 770, a(-768) =
 *            4, a(-767) = 770, a(-766) = 4; a(n) = 0 for n <= -770. Offset =
 *            1 (a(1) is the first listed term)
 * Range:     u8 none, u16 n <= 2210, u32 n <= 8520, u64 n <= 8520,
 *            u128 n <= 8520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N     8520
#define OFFSET    770
#define ARRAY_SZ  (MAX_N + OFFSET + 1)

static seq_t seq[ARRAY_SZ];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise the only given negative terms */
        seq[0] = 0;    /* a(-770) */
        seq[1] = 770;  /* a(-769) */
        seq[2] = 4;    /* a(-768) */
        seq[3] = 770;  /* a(-767) */
        seq[4] = 4;    /* a(-766) */

        /* build the sequence up to n = 8520 */
        for (idx_t i = 5; i < ARRAY_SZ; ++i) {
            seq_t n1 = seq[i - 1];
            seq_t n2 = seq[i - 2];
            seq_t v1 = 0, v2 = 0;

            if (i > (idx_t)n1)
                v1 = seq[i - (idx_t)n1];
            if (i > (idx_t)n2)
                v2 = seq[i - (idx_t)n2];

            seq[i] = v1 + v2;
        }
        initialized = true;
    }

    /* out‑of‑range values simply return 0 – the sequence is defined for
       1 ≤ n ≤ 8520. */
    if (n > MAX_N)
        return 0;

    return seq[(idx_t)n + (idx_t)OFFSET];
}

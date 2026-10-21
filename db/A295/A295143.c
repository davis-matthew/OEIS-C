/*
 * A295143  Solution of the complementary equation a(n) = 2*a(n-1) + a(n-2) +
 *          b(n-1), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + b(n-1); a(0) = 1, a(1) = 2, b(0) = 3,
 *            b(1) = 4; The sequences a( ) and b( ) are increasing
 *            complementary sequences
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Allocate space for the two sequences up to index n. */
    seq_t *A = (seq_t *)malloc((n + 2) * sizeof(seq_t));
    seq_t *B = (seq_t *)malloc((n + 2) * sizeof(seq_t));
    if (!A || !B) {
        /* In case of memory failure return 0 (should not happen in tests). */
        free(A);
        free(B);
        return (seq_t)0;
    }

    /* Initial values. */
    A[0] = (seq_t)1;      /* a(0) */
    A[1] = (seq_t)2;      /* a(1) */
    B[0] = (seq_t)3;      /* b(0) */
    B[1] = (seq_t)4;      /* b(1) */

    /* The last value assigned to the b‑sequence. */
    idx_t last_b = 1;          /* index of the last computed b value */
    seq_t last_b_val = B[1];   /* numeric value of last assigned b */

    /* Pointer used while scanning integers to find the next b‑value. */
    idx_t paIdx = 0;   /* index into A that is the smallest unused a-value */

    /* Compute a(2) directly, because a(2) depends only on the known b(1). */
    A[2] = (seq_t)(( (unsigned __int128)2 * A[1] + A[0] + B[1] ));

    /* Main loop: compute the sequences up to index n. */
    for (idx_t i = 3; i <= n; ++i) {
        /* -------------  Compute b(i-1)  ------------- */
        seq_t candidate = last_b_val + (seq_t)1;  /* smallest unused integer so far */

        while (1) {
            /* Advance paIdx past any a-values that are smaller than the candidate. */
            while (paIdx <= (i - 1) && A[paIdx] < candidate)
                ++paIdx;

            /* If the candidate equals an a-value (and that a-value is within the
               current a range), skip it; otherwise it is the next b-value. */
            if (paIdx <= (i - 1) && A[paIdx] == candidate) {
                ++candidate;                 /* skip this integer */
                continue;
            }
            /* Found the next b-value. */
            B[i - 1] = candidate;
            last_b_val = candidate;
            ++last_b;
            break;
        }

        /* -------------  Compute a(i)  ------------- */
        A[i] = (seq_t)( ( (unsigned __int128)2 * A[i - 1]
                          + A[i - 2]
                          + B[i - 1] ) );
    }

    seq_t result = A[n];
    free(A);
    free(B);
    return result;
}

/*
 * A294563  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) - b(n-2) + n, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) - b(n-2) + n, a(0) = 1, a(1) =
 *            2, b(0) = 3. The sequences (a) and (b) are complementary
 *            increasing sequences satisfying this recurrence
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   grow_seq_buf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/grow_seq_buf.h"

static seq_t *aseq = NULL;   /* values of a */
static size_t acap = 0;      /* capacity of aseq */
static idx_t an = 0;         /* number of elements in aseq */

static seq_t *bseq = NULL;   /* values of b */
static size_t bcap = 0;      /* capacity of bseq */
static idx_t bn = 0;         /* number of elements in bseq */

static idx_t skip_a = 0;     /* next a index to skip when searching b */

seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (an == 0)
    {
        grow_seq_buf(&aseq, &acap, 2);
        grow_seq_buf(&bseq, &bcap, 2);

        aseq[0] = (seq_t)1;
        aseq[1] = (seq_t)2;
        an = 2;

        bseq[0] = (seq_t)3;

        /* compute b[1] */
        seq_t cand = bseq[0] + (seq_t)1; /* 4 */
        skip_a = 0;
        while (skip_a < an)
        {
            if (aseq[skip_a] < cand)
                skip_a++;
            else if (aseq[skip_a] == cand)
            {
                cand++;
                skip_a++;
            }
            else
                break;
        }
        bseq[1] = cand;
        bn = 2;
    }

    /* Extend sequences if needed */
    if (n >= an)
    {
        for (idx_t i = an; i <= n; ++i)
        {
            /* Recurrence: a(i) = a(i-1) + a(i-2) + b(i-1) - b(i-2) + i */
            seq_t ai = aseq[i-1] + aseq[i-2] + bseq[i-1] - bseq[i-2] + (seq_t)i;

            grow_seq_buf(&aseq, &acap, an + 1);
            aseq[an++] = ai;

            /* Find next unused integer to use as b(i) */
            seq_t cand = bseq[i-1] + (seq_t)1;
            while (skip_a < an)
            {
                if (aseq[skip_a] < cand)
                    skip_a++;
                else if (aseq[skip_a] == cand)
                {
                    cand++;
                    skip_a++;
                }
                else
                    break;
            }

            grow_seq_buf(&bseq, &bcap, bn + 1);
            bseq[bn++] = cand;
        }
    }

    return aseq[n];
}

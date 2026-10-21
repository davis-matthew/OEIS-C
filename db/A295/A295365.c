/*
 * A295365  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + b(n-3), where a(0) = 1, a(1) = 2, a[2] = 3, b(0)
 *          = 4, b(1) = 5, b(2) = 6, and (a(n)) and (b(n)) are increasing
 *          complementary sequences.
 * https://oeis.org/A295365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) + b(n-3), a(0)=1,
 *            a(1)=2, a(2)=3, b(0)=4, b(1)=5, b(2)=6 Sequences a() and b() are
 *            complementary increasing sequences
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;
static seq_t *B = NULL;
static idx_t size = 0;

/* Grow cache to at least n+1 terms */
static void ensure_cache(idx_t n)
{
    if (n < size)
        return;

    /* Allocate or grow arrays */
    size_t needed = (size_t)n + 1;
    if (!A || !B)
    {
        A = (seq_t *)malloc(needed * sizeof(seq_t));
        B = (seq_t *)malloc(needed * sizeof(seq_t));
    }
    else
    {
        A = (seq_t *)realloc(A, needed * sizeof(seq_t));
        B = (seq_t *)realloc(B, needed * sizeof(seq_t));
    }
    /* Compute new terms sequentially */
    for (idx_t i = size; i <= n; ++i)
    {
        if (i == 0)
        {
            A[0] = 1;
            B[0] = 4;
        }
        else if (i == 1)
        {
            A[1] = 2;
            B[1] = 5;
        }
        else if (i == 2)
        {
            A[2] = 3;
            B[2] = 6;
        }
        else
        {
            /* Recurrence for a(i) */
            A[i] = A[i-1] + A[i-2] + B[i-1] + B[i-2] + B[i-3];

            /* Compute b(i): smallest integer > b(i-1) not in a[0..i] */
            seq_t cand = B[i-1] + 1;
            while (1)
            {
                int found = 0;
                for (idx_t j = 0; j <= i; ++j)
                    if (cand == A[j]) { found = 1; break; }
                if (!found)
                    break;
                cand++;
            }
            B[i] = cand;
        }
    }
    size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return A[n];
}

/*
 * A309136  Nonnegative integers of the form x^2+15*x*y-y^2.
 * https://oeis.org/A309136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 1268, u64 n <= 1268,
 *            u128 n <= 1268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nonnegative integers of the form x^2 + 15xy - y^2,
   sorted ascendingly to generate OEIS A309136. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* comparator for qsort on seq_t values */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb)
        return -1;
    if (va > vb)
        return 1;
    return 0;
}

/* static storage for the first 10 000 sequence values */
static seq_t a_seq[10000];
static bool seq_inited = false;

static void init_sequence(void)
{
    if (seq_inited)
        return;

    /* bound on x that is sufficient to obtain 10 000 terms */
    const idx_t XMAX = 500;
    /* initial capacity for candidate values */
    const size_t INIT_CAP = 2000000;
    seq_t *vec = (seq_t *)malloc(INIT_CAP * sizeof(seq_t));
    size_t veclen = 0;
    size_t veccap = INIT_CAP;

    /* maximum y for which the expression can be non‑negative:
       y <= ((15 + sqrt(229))/2) * x  */
    const double ALPHA = (15.0 + sqrt(229.0)) / 2.0;

    for (idx_t x = 0; x <= XMAX; ++x)
    {
        idx_t ymax = (idx_t)(ALPHA * (double)x + 1e-9);
        for (idx_t y = 0; y <= ymax; ++y)
        {
            long long tmp = (long long)x * (long long)x
                           + 15LL * (long long)x * (long long)y
                           - (long long)y * (long long)y;
            if (tmp >= 0)
            {
                if (veclen == veccap)
                {
                    veccap *= 2;
                    vec = (seq_t *)realloc(vec, veccap * sizeof(seq_t));
                }
                vec[veclen++] = (seq_t)tmp;
            }
        }
    }

    qsort(vec, veclen, sizeof(seq_t), cmp_seq_t);

    /* collect the first 10 000 unique values */
    size_t needed = 10000;
    size_t idx = 0, uniq = 0;
    seq_t prev = 0;
    bool first = true;

    while (idx < veclen && uniq < needed)
    {
        seq_t cur = vec[idx++];
        if (first || cur != prev)
        {
            a_seq[uniq++] = cur;
            prev = cur;
            first = false;
        }
    }

    free(vec);
    seq_inited = true;
}

seq_t a(idx_t n)
{
    init_sequence();
    return a_seq[n - 1];
}

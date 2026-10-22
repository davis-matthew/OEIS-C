/*
 * A343326  Number of ways to write n as the integral part of (a^3+b^3)/2 +
 *          (c^3+d^3)/6, where a,b,c,d are nonnegative integers with a >=
 *          max{b,1} and c >= max{d,1}.
 * https://oeis.org/A343326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to write n as the integral part of (a³+b³)/2 + (c³+d³)/6,
   a,b,c,d non‑negative integers with a ≥ max(b,1) and c ≥ max(d,1).  */
#include <stdlib.h>
#include <stddef.h>

/* Integer cube root floor for uint64_t values. */
static idx_t cube_root_floor_idx(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) * (r + 1) <= n)
    {
        ++r;
    }
    return r;
}

/* Cube of an integer, result stored in seq_t. */
static seq_t cube(idx_t x)
{
    seq_t v = (seq_t)x;
    return v * v * v;
}

seq_t a(idx_t n)
{
    /* Limits for x = a³+b³ and y = c³+d³.  */
    seq_t maxX = 2 * (seq_t)n + 1;        /*  a³+b³ ≤ 2n+1  */
    seq_t maxY = 6 * (seq_t)n + 5;        /*  c³+d³ ≤ 6n+5  */
    size_t sizeX = (size_t)(maxX + 1);
    size_t sizeY = (size_t)(maxY + 1);

    /* Allocate frequency arrays.  */
    seq_t *freqX = (seq_t *)calloc(sizeX, sizeof(seq_t));
    seq_t *freqY = (seq_t *)calloc(sizeY, sizeof(seq_t));
    if (!freqX || !freqY)
    {
        free(freqX);
        free(freqY);
        return 0;
    }

    /* Fill freqX: counts of a³+b³ with a>=max(b,1).  */
    idx_t a_max = cube_root_floor_idx((idx_t)maxX);
    for (idx_t a = 1; a <= a_max; ++a)
    {
        seq_t a3 = cube(a);
        for (idx_t b = 0; b <= a; ++b)
        {
            seq_t s = a3 + cube(b);
            if (s > maxX)
                break; /* further b will only increase s */
            ++freqX[(size_t)s];
        }
    }

    /* Fill freqY: counts of c³+d³ with c>=max(d,1).  */
    idx_t c_max = cube_root_floor_idx((idx_t)maxY);
    for (idx_t c = 1; c <= c_max; ++c)
    {
        seq_t c3 = cube(c);
        for (idx_t d = 0; d <= c; ++d)
        {
            seq_t s = c3 + cube(d);
            if (s > maxY)
                break;
            ++freqY[(size_t)s];
        }
    }

    /* Prefix sums of freqY to allow range queries.  */
    seq_t *prefY = (seq_t *)malloc(sizeY * sizeof(seq_t));
    if (!prefY)
    {
        free(freqX);
        free(freqY);
        return 0;
    }
    prefY[0] = freqY[0];
    for (size_t i = 1; i < sizeY; ++i)
        prefY[i] = prefY[i - 1] + freqY[i];

    /* Count pairs (x,y) with 6n ≤ 3x + y ≤ 6n+5.  */
    seq_t ans = 0;
    seq_t six_n = 6 * (seq_t)n;
    for (size_t sx = 0; sx < sizeX; ++sx)
    {
        seq_t cntX = freqX[sx];
        if (!cntX)
            continue;

        seq_t three_s = 3 * (seq_t)sx;

        /* y must satisfy: max(0,6n-3x) ≤ y ≤ min(maxY,6n+5-3x).  */
        seq_t y_min = (six_n > three_s) ? (six_n - three_s) : 0;
        seq_t y_max = 6 * (seq_t)n + 5 - three_s;

        if (y_min > y_max)
            continue;        /* empty interval */

        if (y_min > maxY)
            continue;        /* interval starts beyond maxY */

        if (y_max > maxY)
            y_max = maxY;

        size_t i_min = (size_t)y_min;
        size_t i_max = (size_t)y_max;

        seq_t cntY = (i_min == 0) ? prefY[i_max] : prefY[i_max] - prefY[i_min - 1];
        ans += cntX * cntY;
    }

    /* Clean up.  */
    free(freqX);
    free(freqY);
    free(prefY);

    return ans;
}

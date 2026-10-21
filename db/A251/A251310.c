/*
 * A251310  Number of (n+1) X (1+1) 0..1 arrays with no 2 X 2 subblock having
 *          the maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251310 – Number of (n+1)×2 0..1 arrays with the given property
 *  Empirical recurrence:
 *      a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4)   (n ≥ 5)
 *  Initial values:
 *      a(1) = 10
 *      a(2) = 25
 *      a(3) = 61
 *      a(4) = 149
 *  The implementation memoises the values in a dynamically resized
 *  static array so that calls with arbitrary n arrive in any order.
 */

#include <stdlib.h>

static void ensure_capacity(size_t needed, seq_t **buffer, size_t *cap)
{
    if (needed < *cap)
        return;
    size_t newcap = *cap ? *cap : 8;
    while (newcap <= needed)
        newcap <<= 1;
    /* cast of malloc/realloc results is required for C++ */
    seq_t *newbuf = (seq_t *)realloc(*buffer, newcap * sizeof(seq_t));
    if (!newbuf)
        abort();
    *buffer = newbuf;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* sequence is defined for n ≥ 1; the offset is 1 */
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined case, just to be safe */

    size_t idx = (size_t)(n - 1); /* convert to 0‑based index */

    /* static memoisation state */
    static seq_t *buffer = NULL;
    static size_t size  = 0; /* number of elements stored */
    static size_t cap   = 0; /* capacity of the buffer */

    /* initialise the first four values on the first call */
    if (size == 0)
    {
        buffer = (seq_t *)malloc(8 * sizeof(seq_t));
        if (!buffer)
            abort();
        buffer[0] = (seq_t)10;
        buffer[1] = (seq_t)25;
        buffer[2] = (seq_t)61;
        buffer[3] = (seq_t)149;
        size = 4;
        cap = 8;
    }

    /* compute missing terms if needed */
    if (idx >= size)
    {
        ensure_capacity(idx + 1, &buffer, &cap);

        for (size_t i = size; i <= idx; ++i)
        {
            /* recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) */
            seq_t val = (seq_t)3 * buffer[i - 1]
                      - (seq_t)2 * buffer[i - 2]
                      + (seq_t)2 * buffer[i - 3]
                      - buffer[i - 4];
            buffer[i] = val;
        }
        size = idx + 1;
    }

    return buffer[idx];
}

/*
 * A248376  Maximal gap between quadratic residues mod n; here quadratic
 *          residues must be coprime to n.
 * https://oeis.org/A248376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this sequence is defined by a(n) = maximum distance between
 *            consecutive quadratic residues mod n that are coprime to n,
 *            counted circularly around the modulus. The algorithm enumerates
 *            all such residues and then scans for the largest gap
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for n >= 1.  For n = 1 the definition gives a(1) = 1. */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Allocate a boolean array of size n to mark the residues that
       appear.  In C++ we need an explicit cast because calloc returns
       void*. */
    size_t len = (size_t)n;
    bool *seen = (bool *)calloc(len, sizeof(bool));
    if (!seen)
        return 0;   /* allocation failed */

    /* Mark residues y = x^2 mod n for each x coprime to n.  The
       multiplication is performed in unsigned __int128 to avoid
       overflow for 64‑bit indices. */
    for (idx_t x = 1; x < n; ++x)
    {
        /* gcd(x, n) = 1 ? */
        idx_t a = x, b = n;
        while (b)
        {
            idx_t t = a % b;
            a = b;
            b = t;
        }
        if (a != 1)
            continue;   /* not coprime */

        idx_t y = (idx_t)(( (unsigned __int128)x * x ) % (unsigned __int128)n);
        seen[(size_t)y] = true;
    }

    /* Scan the circle to find the maximal gap. */
    bool found = false;
    idx_t first_res = 0, prev_res = 0;
    seq_t max_gap = 0;

    for (idx_t i = 0; i < n; ++i)
    {
        if (seen[(size_t)i])
        {
            if (!found)
            {
                found = true;
                first_res = prev_res = i;
            }
            else
            {
                seq_t gap = (seq_t)(i - prev_res);
                if (gap > max_gap)
                    max_gap = gap;
                prev_res = i;
            }
        }
    }

    /* Wrap‑around gap from the last residue back to the first. */
    seq_t wrap_gap = (seq_t)((n - prev_res) + first_res);
    if (wrap_gap > max_gap)
        max_gap = wrap_gap;

    free(seen);
    return max_gap;
}

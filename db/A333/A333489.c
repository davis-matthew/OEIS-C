/*
 * A333489  Numbers k such that the k-th composition in standard order is an
 *          anti-run (no adjacent equal parts).
 * https://oeis.org/A333489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th index k such that the k-th composition in
 *            standard order is an anti‐run (no adjacent equal parts)
 * Range:     u8 n <= 93, u16 n <= 8259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9077 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_anti_run(idx_t k)
{
    if (k == 0)
        return true;              /* empty composition is an anti‑run */

    /* store the differences between successive positions of 1 bits in the
       reversed binary expansion (i.e., positions of set bits in k, counted
       from the least significant bit, 1‑based). */
    idx_t parts[64];
    int   m = 0;
    idx_t prev_pos = 0;

    for (idx_t i = 0; i < 64; ++i)
    {
        if ((k >> i) == 0)
            break;
        if (k & ((idx_t)1 << i))
        {
            idx_t pos = i + 1;               /* 1‑based position */
            parts[m++] = pos - prev_pos;    /* part length */
            prev_pos = pos;
        }
    }

    /* The final composition is the reverse of the differences array.
       We only need to check that no two consecutive parts are equal. */
    for (int idx = m - 1; idx > 0; --idx)
        if (parts[idx] == parts[idx - 1])
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 1, ...   (offset 1) */
    idx_t count = 0;
    idx_t k     = 0;

    while (true)
    {
        if (is_anti_run(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}

/*
 * A349051  Numbers k such that the k-th composition in standard order is an
 *          alternating permutation of {1..k} for some k.
 * https://oeis.org/A349051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th k such that the k-th composition in standard
   order is an alternating permutation of {1..m} (i.e., the parts of
   the composition are a permutation of the first m positive integers
   and form an alternating sequence). This is the OEIS sequence
   A349051.  The function enumerates k = 0,1,2,... until the n-th
   valid index is found. */

#include <stdint.h>
#include <stdbool.h>

static bool is_good_k(idx_t k)
{
    /* Step 1: collect positions of 1's in the reversed binary expansion
       of k, using 1‑based indices.  Since k is up to 64 bits the
       maximum number of positions is 64. */
    unsigned int pos[64];
    int m = 0;
    idx_t tmp = k;
    int bitpos = 1;                 /* 1‑based position in reversed bits   */
    while (tmp) {
        if (tmp & 1ULL)
            pos[m++] = bitpos;
        tmp >>= 1;
        bitpos++;
    }
    /* k == 0 → empty composition, which is considered perfect. */
    if (m == 0)
        return true;

    /* Step 2: build the first‑difference array f. */
    unsigned int f[64];
    f[0] = pos[0];
    for (int i = 1; i < m; ++i)
        f[i] = pos[i] - pos[i - 1];

    /* Step 3: reverse f to obtain the composition parts. */
    unsigned int part[64];
    for (int i = 0; i < m; ++i)
        part[i] = f[m - 1 - i];

    /* Step 4: check that the parts form a permutation of {1..m}. */
    if (m > 64)   /* safety, should never happen. */
        return false;
    bool seen[65] = {0};
    unsigned int maxv = 0;
    for (int i = 0; i < m; ++i) {
        unsigned int x = part[i];
        if (x == 0 || x > (unsigned)m)
            return false;          /* values must be in 1..m                */
        if (seen[x]) return false; /* no duplicates                        */
        seen[x] = true;
        if (x > maxv)
            maxv = x;
    }
    if (maxv != (unsigned)m)        /* missing some value                  */
        return false;

    /* Step 5: check alternation.  For len <= 2 any sequence is alternating. */
    if (m <= 2)
        return true;

    bool ok0 = true, ok1 = true;    /* ok0: pattern < > < > ...  ok1: > < > < ... */
    for (int i = 0; i < m - 1; ++i) {
        bool lt = part[i] < part[i + 1];
        if (i % 2 == 0) {
            if (!lt) ok0 = false;
            if (lt)  ok1 = false;
        } else {
            if (lt)  ok0 = false;
            if (!lt) ok1 = false;
        }
        if (!ok0 && !ok1)
            return false;
    }
    return ok0 || ok1;
}

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 1, a(3) = 5, ...   */
    idx_t count = 0;
    idx_t k = 0;
    while (true) {
        if (is_good_k(k))
            ++count;
        if (count == n)
            return (seq_t)k;
        ++k;
    }
}

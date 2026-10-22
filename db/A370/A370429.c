/*
 * A370429  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, n OR a(n) is a binary
 *          palindrome (where OR denotes the bitwise OR operator).
 * https://oeis.org/A370429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Lexicographically earliest sequence of distinct nonnegative
 *            integers such that for any n ≥ 0, n OR a(n) is a binary
 *            palindrome. The algorithm pairs each unpaired i with the
 *            smallest unused j ≥ i for which i|j is a binary palindrome. The
 *            permutation is built on demand and stored in static arrays
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* two types are provided by the build system:
   seq_t  – the value type (unsigned integer or arbitrary‑precision class)
   idx_t  – 64‑bit unsigned integer (uint64_t)                     */

/* memoisation tables */
static seq_t *perm      = NULL;   /* a[i] (0 means unassigned) */
static bool  *used      = NULL;   /* has a[i] been assigned?    */
static size_t cap        = 0;      /* current capacity of tables  */
static size_t next_computed = 0;   /* smallest index not yet processed */

/* enlarge tables so that indices 0..needed are valid */
static void ensure_size(size_t needed)
{
    if (needed < cap)
        return;
    size_t new_cap = cap ? cap << 1 : 8;
    while (new_cap <= needed)
        new_cap <<= 1;
    perm = (seq_t *)realloc(perm, new_cap * sizeof(seq_t));
    used = (bool *)realloc(used, new_cap * sizeof(bool));
    for (size_t i = cap; i < new_cap; ++i)
    {
        used[i] = false;
        perm[i] = (seq_t)0;
    }
    cap = new_cap;
}

/* number of bits needed to represent x (at least 1) */
static unsigned bitlen(seq_t x)
{
    if (x == (seq_t)0)
        return 1;
    unsigned len = 0;
    seq_t t = x;
    while (t != (seq_t)0)
    {
        ++len;
        t >>= 1;
    }
    return len;
}

/* test whether the binary representation of x is a palindrome */
static bool is_bin_pal(const seq_t x)
{
    unsigned len = bitlen(x);
    seq_t one = (seq_t)1;
    for (unsigned i = 0; i < len / 2; ++i)
    {
        seq_t low  = (x >> i) & one;
        seq_t high = (x >> (len - 1 - i)) & one;
        if (low != high)
            return false;
    }
    return true;
}

/* required function */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;        /* n is small enough for size_t in tests */
    ensure_size(idx);

    if (idx >= next_computed)
    {
        for (size_t i = next_computed; i <= idx; ++i)
        {
            if (used[i])           /* already paired as the partner of a smaller element */
                continue;

            size_t j = i;
            while (true)
            {
                ensure_size(j);
                if (used[j])       /* j already taken */
                {
                    ++j;
                    continue;
                }
                if (is_bin_pal(((seq_t)i) | ((seq_t)j)))
                {
                    perm[i] = (seq_t)j;
                    perm[j] = (seq_t)i;
                    used[i] = true;
                    used[j] = true;
                    break;
                }
                ++j;
            }
        }
        next_computed = idx + 1;
    }

    return perm[idx];
}

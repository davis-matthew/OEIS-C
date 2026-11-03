/*
 * A278580  Numbers n such that Jacobi(n,23) = 1.
 * https://oeis.org/A278580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 907 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th positive integer such that Jacobi(n,23) = 1
   (i.e. n is a quadratic residue mod 23). */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Bitmask of quadratic residues modulo 23 (excluding 0).  Bits are
   set for residues that yield Jacobi(n,23) = 1.  This set is:
   1, 2, 3, 4, 6, 8, 9, 12, 13, 16, 18. */
static const uint32_t RESIDUE_MASK =
    (1u << 1)  |
    (1u << 2)  |
    (1u << 3)  |
    (1u << 4)  |
    (1u << 6)  |
    (1u << 8)  |
    (1u << 9)  |
    (1u << 12) |
    (1u << 13) |
    (1u << 16) |
    (1u << 18);

/* Return the n-th element of the sequence (offset 1).  For n <= 0 the
   behaviour is undefined; we simply return 0. */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;   /* undefined for 0 or negative */

    idx_t count = 0;        /* how many valid numbers found so far */
    seq_t value = 1;        /* candidate integer to test */

    for (;; ++value)
    {
        /* Compute residue modulo 23 */
        uint64_t r = (uint64_t)(value % 23);
        if (r != 0 && (RESIDUE_MASK & (1u << r)))
        {
            ++count;
            if (count == n) return value;
        }
    }
}

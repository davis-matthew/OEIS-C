/*
 * A334819  Largest quadratic nonresidue modulo n (with n >= 3).
 * https://oeis.org/A334819
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: largest quadratic nonresidue modulo n
   For each n ≥ 3, returns the maximum k < n such that
   no integer x satisfies x² ≡ k (mod n).          */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Not defined for n < 3 – return 0 */
    if (n < 3)
        return (seq_t)0;

    /* sz is the number of residues modulo n */
    size_t sz = (size_t)n;

    /* Allocate boolean array for residues */
    bool *seen = (bool *)malloc(sz);
    if (!seen)
        return (seq_t)0;  /* allocation failed */

    /* Initialize to false */
    for (size_t i = 0; i < sz; ++i)
        seen[i] = false;

    /* Mark all quadratic residues */
    for (size_t i = 0; i < sz; ++i) {
        unsigned __int128 prod = (unsigned __int128)i * i;
        size_t r = (size_t)(prod % sz);
        seen[r] = true;
    }

    /* Find the largest non‑residue */
    seq_t result = (seq_t)0;
    for (int64_t k = (int64_t)sz - 1; k >= 0; --k) {
        if (!seen[(size_t)k]) {
            result = (seq_t)k;
            break;
        }
    }

    free(seen);
    return result;
}

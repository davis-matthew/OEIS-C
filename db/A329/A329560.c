/*
 * A329560  BII-numbers of antichains of sets with empty intersection.
 * https://oeis.org/A329560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth BII-number whose set‑system is an antichain with
 *            empty intersection
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool is_valid_bii(idx_t k)
{
    idx_t indices[64];
    size_t m = 0;
    idx_t temp = k;
    idx_t pos = 1;                  /* bit positions start at 1 */
    while (temp) {
        if (temp & 1) {
            indices[m++] = pos;
        }
        temp >>= 1;
        pos++;
    }
    if (m == 0) return true;        /* the empty set‑system is considered valid */

    seq_t masks[64];
    for (size_t ii = 0; ii < m; ++ii) {
        idx_t val = indices[ii];
        seq_t mask = 0;
        idx_t bitpos = 1;
        while (val) {
            if (val & 1) {
                mask |= ((seq_t)1 << (bitpos - 1));
            }
            val >>= 1;
            bitpos++;
        }
        masks[ii] = mask;
    }

    /* empty intersection */
    seq_t inter = ~((seq_t)0);
    for (size_t ii = 0; ii < m; ++ii) {
        inter &= masks[ii];
        if (inter == 0) break;
    }
    if (inter != 0) return false;

    /* antichain: no proper subset relation */
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = i + 1; j < m; ++j) {
            seq_t a = masks[i];
            seq_t b = masks[j];
            if (a == b) continue;
            if ((a & ~b) == 0 || (b & ~a) == 0) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, though n should be >= 1 */
    idx_t count = 0;
    for (idx_t val = 0; ; ++val) {
        if (is_valid_bii(val)) {
            ++count;
            if (count == n) {
                return (seq_t)val;
            }
        }
    }
}

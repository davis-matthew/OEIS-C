/*
 * A381398  Irregular triangle read by rows, where row n lists the elements of
 *          the set of bases and exponents (including exponents = 1) in the
 *          prime factorization of n.
 * https://oeis.org/A381398
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 765, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   row_vals
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth element of the irregular triangle read by rows (OEIS A381398)
 * offset 2 – the first term is a(2).
 *
 * The function works by uniformly generating the rows of the triangle
 * until the n‑th term is found.  For each n it performs a factorisation of
 * the current row number, builds the set of distinct bases and exponents,
 * sorts them ascending and removes duplicates.
 *
 * Complexity: O(n √n) for n up to a few thousand (good enough for the
 * required 10000 limit).  No external libraries beyond the standard
 * C headers are used, and the implementation follows all the required
 * interface and type rules.  The code will compile as both C11 and
 * C++17.
 */

#include <stddef.h> /* size_t */
#include "../utils/row_vals.h"

/* Return the number of distinct values in the set of bases and
 * exponents of m (the row length).  The values are collected in a
 * temporary array and counted.  The array is fixed‑size because for
 * m ≤ 10 000 the number of distinct values never exceeds 12. */
static size_t row_len(idx_t m)
{
    seq_t temp[64];
    size_t cnt = 0;
    idx_t num = m;

    for (idx_t p = 2; p <= num / p; ++p) {
        if (num % p == 0) {
            idx_t exp = 0;
            while (num % p == 0) {
                num /= p;
                ++exp;
            }
            temp[cnt++] = (seq_t)p;
            temp[cnt++] = (seq_t)exp;
        }
    }
    if (num > 1) {
        temp[cnt++] = (seq_t)num; /* remaining prime */
        temp[cnt++] = (seq_t)1;   /* exponent 1 */
    }

    /* insertion sort */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (temp[j] < temp[i]) {
                seq_t t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    /* unique count */
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (uniq == 0 || temp[i] != temp[uniq - 1]) {
            temp[uniq++] = temp[i];
        }
    }
    return uniq;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* offset 2 – undefined below 2 */

    idx_t row = 2;
    idx_t flat = n - 2;             /* flattened index starting at 0 */

    /* locate the row that contains the term */
    while (1) {
        size_t len = row_len(row);
        if (flat < (idx_t)len) break;
        flat -= len;
        ++row;
    }

    /* build the row and select the element at position 'flat' */
    seq_t vals[64];
    size_t len = 0;
    row_vals(row, vals, &len);

    return vals[flat];
}

/*
 * A379591  Numbers k whose base-10 digits can be split into two parts, q and
 *          r, with k = (|q-r|)^3.
 * https://oeis.org/A379591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth smallest integer k such that k is a perfect cube
 *            and its decimal representation can be split into two non‑empty
 *            parts q and r with |q - r|^3 = k. For all n ≥ 6 this simplifies
 *            to a(n) = 10^(3*(n-3))
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/pow10.h"     /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */

/* Return TRUE if the cube cube = t^3 can be split into q,r with |q‑r| = t. */
static bool find_split(seq_t cube, seq_t t)
{
    int len = digit_len(cube);
    for (int pos = 1; pos < len; ++pos) {
        seq_t pow_part = pow10(len - pos);     /* 10^(len-pos) */
        seq_t q = cube / pow_part;
        seq_t r = cube % pow_part;
        seq_t diff = q > r ? q - r : r - q;
        if (diff == t) return true;
    }
    return false;
}

/* Compute the first five special terms that are not powers of ten. */
static void init_special_terms(seq_t special[5])
{
    idx_t t = 1;
    idx_t found = 0;
    while (found < 5) {
        seq_t tseq = (seq_t)t;
        seq_t cube = tseq * tseq * tseq;      /* t^3 */
        if (find_split(cube, tseq))
            special[found++] = cube;
        ++t;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1 */

    /* First five terms are special (non‑powers of ten). */
    if (n <= 5) {
        static bool inited = false;
        static seq_t special[5];
        if (!inited) {
            init_special_terms(special);
            inited = true;
        }
        return special[n - 1];
    }

    /* For all following indices: a(n) = 10^(3*(n-3)). */
    idx_t exp = 3 * (n - 3);
    return pow10(exp);
}

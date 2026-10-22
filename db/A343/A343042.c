/*
 * A343042  Array T(n, k), n, k >= 0, read by antidiagonals; lunar
 *          multiplication table for the factorial base.
 * https://oeis.org/A343042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 362, u16 n <= 9267, u32 n <= 9267, u64 n <= 9267,
 *            u128 n <= 9267, big any n
 * Helpers:   coords, factorial_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A343042 – Lunar factorial‑base multiplication table
 *
 * The table entry T(n,k) is defined as:
 *   Let a_p (p=1..L) and b_q (q=1..M) be the factorial‑base digits of n
 *   and k, where the least significant digit is a_1 or b_1.
 *
 *   For each position s = 1 .. L+M-1:
 *     δ_s = max{ min(a_p , b_q) | p+q = s+1 } .
 *
 *   Then
 *     T(n,k) = Σ_{s=1}^{L+M-1} s! · δ_s .
 *
 *   The sequence is read by antidiagonals of the symmetric table T(n,k),
 *   so that a(0)=T(0,0), a(1)=T(1,0), a(2)=T(2,0), a(3)=T(3,0), a(4)=T(1,1), …
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/coords.h"
#include "../utils/factorial_base.h"

/* Compute T(n,k) from the definition above. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 0 || k == 0)
        return (seq_t)0;

    /* Factorial base digits (least significant first) */
    uint8_t a[128];
    uint8_t b[128];
    size_t lenA, lenB;

    factorial_base(n, a, &lenA);
    factorial_base(k, b, &lenB);

    size_t maxPos = lenA + lenB - 1;
    seq_t result = (seq_t)0;
    seq_t fact   = (seq_t)1;      /* current factorial s! */

    for (size_t s = 1; s <= maxPos; ++s) {
        fact *= (seq_t)s;         /* s! */

        uint8_t best = 0;
        for (size_t p = 1; p <= lenA; ++p) {
            size_t q = s + 1 - p;
            if (q >= 1 && q <= lenB) {
                uint8_t cand = a[p - 1] < b[q - 1] ? a[p - 1] : b[q - 1];
                if (cand > best)
                    best = cand;
            }
        }
        if (best)
            result += fact * (seq_t)best;
    }
    return result;
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    idx_t i, j;
    coords(n, &i, &j);
    return compute_T(i, j);
}

/*
 * A360415  a(n) is the greatest number k not yet in the sequence such that
 *          A065361(n) = A065361(k).
 * https://oeis.org/A360415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k-th smallest member of the class {j : A065361(j) =
 *            A065361(n)}, where n is the (m+1-k)-th smallest of the m
 *            members; computed by enumerating the class (hyperbinary-style
 *            ternary digit strings) and reversing the order
 * Range:     u8 n <= 147, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Validated: standard, pass, 84 terms through n = 9413 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A065361_val(idx_t n)
{
    seq_t res = 0;
    unsigned int pos = 0;
    while (n > 0) {
        idx_t digit = n % 3;
        res += ((seq_t)digit) << pos;
        n /= 3;
        ++pos;
    }
    return res;
}

static void A360415_gen(seq_t v, seq_t p3, seq_t k, seq_t **arr, size_t *cnt, size_t *cap)
{
    if (v == 0) {
        if (*cnt == *cap) {
            size_t nc = *cap ? *cap * 2 : 16;
            *arr = mem_resize(seq_t, *arr, nc);
            *cap = nc;
        }
        (*arr)[*cnt] = k;
        (*cnt)++;
        return;
    }
    if (v % 2 == 1) {
        A360415_gen((v - 1) / 2, p3 * 3, k + p3, arr, cnt, cap);
    } else {
        A360415_gen(v / 2, p3 * 3, k, arr, cnt, cap);
        A360415_gen((v - 2) / 2, p3 * 3, k + p3 * 2, arr, cnt, cap);
    }
}

seq_t a(idx_t n)
{
    seq_t v = A065361_val(n);
    seq_t *arr = NULL;
    size_t cnt = 0, cap = 0;
    A360415_gen(v, (seq_t)1, (seq_t)0, &arr, &cnt, &cap);

    seq_t nn = (seq_t)n;
    size_t r = 0;
    for (size_t i = 0; i < cnt; i++)
        if (arr[i] < nn) r++;
    size_t t = cnt - 1 - r;

    seq_t result = nn;
    for (size_t i = 0; i < cnt; i++) {
        size_t less = 0;
        for (size_t j = 0; j < cnt; j++)
            if (arr[j] < arr[i]) less++;
        if (less == t) { result = arr[i]; break; }
    }
    mem_free(arr);
    return result;
}

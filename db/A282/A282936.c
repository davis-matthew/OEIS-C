/*
 * A282936  Concatenation of the products of elements of P{d_1}, P{d_2},
 *          P{d_3}, ..., P{d_n}; where P{d_n} denote the power set of the set
 *          of divisors of n ordered by the size of the subsets, and in each
 *          subset, following the increasing order.
 * https://oeis.org/A282936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th product in the sequence obtained by listing, for
 *            k = 1,2,…, the products of all non‑empty subsets of the divisors
 *            of k, ordered first by increasing subset size, then
 *            lexicographically by the elements. The function returns that
 *            n‑th product as seq_t
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Utility: compute and sort all positive divisors of a 64‑bit number.
   divs array must be large enough (at least 128 in this implementation).   */
static void divisors_of(uint64_t n, uint64_t *divs, size_t *cnt) {
    size_t c = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[c++] = d;
            uint64_t e = n / d;
            if (e != d) divs[c++] = e;
        }
    }
    /* Simple insertion sort – the array is small */
    for (size_t i = 0; i < c; ++i)
        for (size_t j = i + 1; j < c; ++j)
            if (divs[i] > divs[j]) {
                uint64_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
    *cnt = c;
}

/* ------------------------------------------------------------------ */
/* Recursive enumeration of subsets of a given size.
   When sz == 0, a complete subset has been built and prod is its product.
   count accumulates the number of products produced so far.
   When the desired index is reached, the product is stored in *val
   and *found is set to true. */
static void rec(const uint64_t *divs, size_t d, size_t sz, size_t start,
                seq_t prod, idx_t *count, idx_t target, seq_t *val,
                bool *found) {
    if (*found) return;

    if (sz == 0) {
        /* One product produced */
        (*count)++;
        if (*count == target) {
            *val = prod;
            *found = true;
        }
        return;
    }

    for (size_t i = start; i <= d - sz; ++i) {
        seq_t next_prod = prod * (seq_t)divs[i];
        rec(divs, d, sz - 1, i + 1, next_prod, count,
            target, val, found);
        if (*found) return;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n) {
    idx_t target = n;          /* 1‑based index */
    idx_t count = 0;           /* number of terms produced so far */
    seq_t result = 0;
    bool found = false;

    uint64_t divs[128];
    size_t divcnt;

    for (uint64_t k = 1; !found; ++k) {
        divisors_of(k, divs, &divcnt);
        for (size_t sz = 1; sz <= divcnt && !found; ++sz) {
            rec(divs, divcnt, sz, 0, (seq_t)1,
                &count, target, &result, &found);
        }
    }

    return result;
}

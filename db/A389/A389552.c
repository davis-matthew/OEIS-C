/*
 * A389552  Vandermonde determinant of the divisors of n.
 * https://oeis.org/A389552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ∏_{1≤i<j≤τ(n)} (d_j – d_i) where d_1, …, d_{τ(n)} are all
 *            divisors of n sorted in increasing order (the Vandermonde
 *            determinant of the divisor set)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   factor_u64, generate_divisors_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/generate_divisors_rec.h"

/* Simple insertion sort – the divisor list is tiny (≤ few hundred). */
static void
insertion_sort(seq_t *a, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        for (size_t j = i; j > 0 && a[j - 1] > a[j]; --j) {
            seq_t tmp = a[j];
            a[j] = a[j - 1];
            a[j - 1] = tmp;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for this sequence, but safe */

    /* Factor n: obtain prime list and exponents. */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Generate divisors.  For n ≤ 10⁴ the number of divisors is < 200. */
    static seq_t divs[2048];
    size_t dpos = 0;
    generate_divisors_rec(0, (seq_t)1, primes, exps, pcnt, divs, &dpos);

    /* Sort them increasingly. */
    insertion_sort(divs, dpos);

    /* Compute the Vandermonde product. */
    seq_t prod = 1;
    for (size_t i = 0; i < dpos; ++i) {
        for (size_t j = i + 1; j < dpos; ++j) {
            seq_t diff = divs[j] - divs[i];
            prod = prod * diff;
        }
    }

    return prod;
}

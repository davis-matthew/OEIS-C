/*
 * A373006  Antidiagonal sums of A373005.
 * https://oeis.org/A373006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} A(i,n), where A(i,j) is the Kleitman-type
 *            bound used by A373005: j odd: A(i,j) = Sum_{t=0..(j-1)/2}
 *            C(i,t); j even: A(i,j) = C(i-1,j/2-1) + Sum_{t=0..j/2-1} C(i,t);
 *            with A(0,0)=1, A(0,j)=1 for odd j, A(0,j)=2 if j/2 is odd and 0
 *            otherwise (j even, j>0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-10)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t A373006_cell(idx_t i, idx_t j) {
    if (j == 0) {
        return (i == 0) ? (seq_t)1 : (seq_t)0;
    }
    if (i == 0) {
        if (j % 2 == 1) return (seq_t)1;
        return (((j / 2) % 2) == 1) ? (seq_t)2 : (seq_t)0;
    }
    seq_t sum = 0;
    if (j % 2 == 1) {
        idx_t t = (j - 1) / 2;
        for (idx_t k = 0; k <= t; ++k) sum += binomial(i, k);
        return sum;
    } else {
        idx_t t = j / 2;
        for (idx_t k = 0; k < t; ++k) sum += binomial(i, k);
        sum += binomial(i - 1, t - 1);
        return sum;
    }
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) {
        total += A373006_cell(i, n);
    }
    return total;
}

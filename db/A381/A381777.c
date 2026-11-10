/*
 * A381777  Self-convolution of A001190.
 * https://oeis.org/A381777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} A001190(k)*A001190(n-k), with A001190(0)=0,
 *            A001190(1)=1, A001190(2k+1) = Sum_{j=1..k} A(j)*A(2k+1-j),
 *            A001190(2k) = Sum_{j=1..k-1} A(j)*A(2k-j) + A(k)*(A(k)+1)/2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 30, u64 n <= 55, u128 n <= 105,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 2464 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A381777_wedderburn_at(const seq_t *m, idx_t i) {
    seq_t result = 0;
    if (i % 2 == 1) {
        idx_t k = (i - 1) / 2;
        for (idx_t j = 1; j <= k; j++) result = result + m[j] * m[i - j];
    } else {
        idx_t k = i / 2;
        for (idx_t j = 1; j < k; j++) result = result + m[j] * m[i - j];
        seq_t ak = m[k];
        seq_t t;
        if (ak % 2 == 0) t = (ak / 2) * (ak + 1);
        else t = ak * ((ak + 1) / 2);
        result = result + t;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t *w = mem_new(seq_t, (size_t)n + 2);
    w[0] = 0;
    w[1] = 1;
    for (idx_t i = 2; i <= n; i++) w[i] = A381777_wedderburn_at(w, i);
    seq_t s = 0;
    for (idx_t k = 0; 2 * k < n; k++) s = s + w[k] * w[n - k];
    s = s * 2;
    if (n % 2 == 0) s = s + w[n / 2] * w[n / 2];
    mem_free(w);
    return s;
}

/*
 * A357078  Triangle read by rows. The partition transform of A355488, which
 *          are the alternating row sums of the number of permutations of [n]
 *          with k components (A059438).
 * https://oeis.org/A357078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = [x^n] B(x)^k, where B(x) = Sum_{m>=0} A355488(m)*x^m,
 *            A355488(m) = m! - 2*Sum_{j=1..m} j!*A355488(m-j), A355488(0)=0;
 *            a(n) is T(r,k) in the row-by-row flattening (row r has r+1
 *            terms, 0<=k<=r)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A357078_a(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;
    size_t R = (size_t)r;

    seq_t *fact = mem_new(seq_t, R + 1);
    seq_t *b = mem_new(seq_t, R + 1);
    seq_t *P = mem_new(seq_t, R + 1);
    seq_t *Q = mem_new(seq_t, R + 1);

    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= R; i++) fact[i] = fact[i - 1] * (seq_t)i;

    b[0] = (seq_t)0;
    for (size_t i = 1; i <= R; i++) {
        seq_t s = (seq_t)0;
        for (size_t j = 1; j <= i; j++) s += fact[j] * b[i - j];
        b[i] = fact[i] - s * (seq_t)2;
    }

    P[0] = (seq_t)1;
    for (idx_t j = 0; j < k; j++) {
        for (size_t i = 0; i <= R; i++) {
            seq_t s = (seq_t)0;
            for (size_t m = 1; m <= i; m++) s += P[i - m] * b[m];
            Q[i] = s;
        }
        for (size_t i = 0; i <= R; i++) P[i] = Q[i];
    }
    seq_t res = P[R];

    mem_free(fact);
    mem_free(b);
    mem_free(P);
    mem_free(Q);
    return res;
}

seq_t a(idx_t n)
{
    return A357078_a(n);
}

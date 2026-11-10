/*
 * A343801  Total sum of the parts in all partitions counted by A339479(n).
 * https://oeis.org/A343801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with F(0,k)=1, F(n,0)=F(n-1,1),
 *            F(n,k)=F(n-1,k+1)+F(n,floor(k/2)) (A339479 recurrence), and
 *            A(0,k)=0, A(n,0)=F(n-1,1)+A(n-1,1),
 *            A(n,k)=F(n-1,k+1)+A(n-1,k+1)+2*A(n,floor(k/2)) for k>0, a(n) =
 *            A(n,0) for n>=1, a(0)=0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 1375 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    size_t W = N + 3;
    seq_t *Fp = mem_new(seq_t, W);
    seq_t *Ap = mem_new(seq_t, W);
    seq_t *Fc = mem_new(seq_t, W);
    seq_t *Ac = mem_new(seq_t, W);
    for (size_t k = 0; k < W; k++) { Fp[k] = (seq_t)1; Ap[k] = (seq_t)0; }
    for (size_t m = 1; m <= N; m++) {
        size_t width = N + 2 - m;  /* max k index */
        Fc[0] = Fp[1];
        Ac[0] = Fp[1] + Ap[1];
        for (size_t k = 1; k <= width; k++) {
            Fc[k] = Fp[k + 1] + Fc[k / 2];
            Ac[k] = Fp[k + 1] + Ap[k + 1] + (Ac[k / 2] << 1);
        }
        seq_t *t;
        t = Fp; Fp = Fc; Fc = t;
        t = Ap; Ap = Ac; Ac = t;
    }
    seq_t res = Ap[0];
    mem_free(Fp);
    mem_free(Ap);
    mem_free(Fc);
    mem_free(Ac);
    return res;
}
